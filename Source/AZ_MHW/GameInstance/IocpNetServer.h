#pragma once
#include "AZWindows.h"
#include "Define.h"
#include "ClientInfo.h"


class IocpNetServer
{
public:
	IocpNetServer(void) = default;
	virtual ~IocpNetServer(void)
	{
		//윈속의 사용을 끝낸다.
		WSACleanup();
	}

	//소켓을 초기화하는 함수
	bool Init(const UINT32 max_io_worker_thread_count)
	{
		WSADATA wsa_data;

		int nRet = WSAStartup(MAKEWORD(2, 2), &wsa_data); // 윈속 초기화
		if (0 != nRet)
		{
			//printf("[에러] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[Error] WSAStartup()Funtion Fail : %d\n"), WSAGetLastError());
			return false;
		}

		//연결지향형 TCP , Overlapped I/O 소켓을 생성
		listen_socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

		if (INVALID_SOCKET == listen_socket_)
		{
			//printf("[에러] socket()함수 실패 : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[Error] socket()Funtion Fail : %d\n"), WSAGetLastError());
			return false;
		}

		max_io_worker_thread_count_ = max_io_worker_thread_count;

		//printf("소켓 초기화 성공\n");
		UE_LOG(LogTemp, Warning, TEXT("Socket Reset Success\n"));
		return true;
	} // end Init

	//------서버용 함수-------//
	//서버의 주소정보를 소켓과 연결시키고 접속 요청을 받기 위해 
	//소켓을 등록하는 함수

	bool BindAndListen(int nBindPort)
	{
		SOCKADDR_IN		stServerAddr;
		stServerAddr.sin_family = AF_INET; // 주소 체계 ipv4 or ipv6
		stServerAddr.sin_port = htons(nBindPort); //서버 포트를 설정한다. htons : short intger(일반적으로 2byte)데이터를 네트워크 byte order로 변경	
		stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); // htonl : long intger(일반적으로 4byte)데이터를 네트워크 byte order로 변경한다.

		//bind : listen_socket_에 stServerAddr주소를 할당
		int nRet = bind(listen_socket_, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
		if (0 != nRet)
		{
			//printf("[에러] bind()함수 실패 : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[Error] bind()Funtion Fail : %d\n"), WSAGetLastError());
			return false;
		}

		// 클라이언트 Connect() 함수 연결 요청에 listen_socket_ 연결 요청을 listen 대기열 추가
		// accept 호출 시에는 listen 대기열에서 가장 오래된 연결 소켓을 가져와서 클라이언트와 통신
		nRet = listen(listen_socket_, 5);
		if (0 != nRet)
		{
			//printf("[에러] listen()함수 실패 : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[Error] listen()Funtion Fail : %d\n"), WSAGetLastError());
			return false;
		}

		// https://www.joinc.co.kr/w/man/4200/CreateIoCompletionPort
		// IOCP 객체 생성 요청
		iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, max_io_worker_thread_count_);
		if (NULL == iocp_handle_)
		{
			//printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[Error] CreateIoCompletionPort()Funtion Fail: %d\n"), GetLastError());
			return false;
		}

		// 1. listen_socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
		// 2. listen_socket과 연결 될 IOCP 객체 (iocp_handle_)
		// Socket을 IOCP에 연결
		auto H_iocp_handle = CreateIoCompletionPort((HANDLE)listen_socket_, iocp_handle_, (UINT32)0, 0);
		if (nullptr == H_iocp_handle)
		{
			//printf("[에러] listen socket IOCP bind 실패 : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[Error] listen socket IOCP bind Fail : %d\n"), WSAGetLastError());
			return false;
		}

		//printf("서버 등록 성공..\n");
		UE_LOG(LogTemp, Warning, TEXT("Server Registration Success..\n"));
		return true;
	}

	bool StartServer(const UINT32 maxClientCount)
	{
		// 클라이언트 구조체 미리 쭉 만들어놓기
		CreateClient(maxClientCount);

		bool bRet = CreateWokerThread();
		if (false == bRet) {
			return false;
		}

		bRet = CreateAccepterThread();
		if (false == bRet) {
			return false;
		}

		//printf("서버 시작\n");
		UE_LOG(LogTemp, Warning, TEXT("Server Start\n"));
		return true;
	}

	//생성되어있는 쓰레드를 파괴한다.
	void DestroyThread()
	{
		// WorkThread 종료
		is_worker_run_ = false;
		CloseHandle(iocp_handle_);

		for (auto& th : io_worker_threads_)
		{
			if (th.joinable())
			{
				th.join();
			}
		}

		// Accepter 쓰레드를 종료한다.
		is_accepter_run_ = false;
		closesocket(listen_socket_);

		if (accepter_thread_.joinable())
		{
			accepter_thread_.join();
		}
	}

	bool SendMsg(const UINT32 client_index, const UINT32 data_size, char* P_send_data)
	{
		auto pClient = GetClientInfo(client_index);
		return pClient->SendMsg(data_size, P_send_data);
	}

	void BroadcastSendMsg(const UINT32 client_index, const UINT32 data_size, char* P_send_data)
	{
		for (auto& broadastList : client_Infos_)
		{
			// 실제 접속이 되었다면
			if (broadastList->IsConnected() == 1)
			{
				auto ConnectedList = GetClientInfo(broadastList->GetIndex());
				ConnectedList->SendMsg(data_size, P_send_data);
			}
		}
	}

	// 네트워크 이벤트를 처리할 함수들
	virtual void OnConnect(const UINT32 clientIndex_) {}
	virtual void OnClose(const UINT32 clientIndex_) {}
	virtual void OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_) {}

private:
	void CreateClient(const UINT32 maxClientCount)
	{
		for (UINT32 i = 0; i < maxClientCount; ++i)
		{
			// 접속 허용 클라이언트 만큼 빈 ClientInfo 클래스 할당
			auto clinet = new ClientInfo();
			clinet->Init(i, iocp_handle_);

			client_Infos_.push_back(clinet);
		}
	}

	//WaitingThread Queue에서 대기할 쓰레드들을 생성
	bool CreateWokerThread()
	{
		//WaingThread Queue에 대기 상태로 넣을 쓰레드들 생성 권장되는 개수 : (cpu개수 * 2) + 1 
		for (UINT32 i = 0; i < max_io_worker_thread_count_; i++)
		{
			io_worker_threads_.emplace_back([this]() { WorkerThread(); });
		}

		//printf("WokerThread 시작..\n");
		UE_LOG(LogTemp, Warning, TEXT("WokerThread Start..\n"));
		return true;
	}

	//사용하지 않는 클라이언트 정보 구조체를 반환한다.
	ClientInfo* GetEmptyClientInfo()
	{
		for (auto& clinet : client_Infos_)
		{
			if (clinet->IsConnected() == false)
			{
				return clinet;
			}
		}
		return nullptr;
	}

	ClientInfo* GetClientInfo(const UINT32 clientIndex)
	{
		return client_Infos_[clientIndex];
	}

	//accept요청을 처리하는 쓰레드 생성
	bool CreateAccepterThread()
	{
		accepter_thread_ = std::thread([this]() { AccepterThread(); });

		//printf("AccepterThread 시작..\n");
		UE_LOG(LogTemp, Warning, TEXT("AccepterThread Start..\n"));
		return true;
	}

	//Overlapped I/O작업에 대한 완료 통보를 받아 
	//그에 해당하는 처리를 하는 함수
	void WorkerThread()
	{
		//CompletionKey를 받을 포인터 변수
		ClientInfo* P_client_info = nullptr;
		//함수 호출 성공 여부
		// FIXME 임시
		BOOL bSuccess = 1;
		//Overlapped I/O작업에서 전송된 데이터 크기
		DWORD dwIoSize = 0;
		//I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
		LPOVERLAPPED lpOverlapped = NULL;

		while (is_worker_run_)
		{
			bSuccess = GetQueuedCompletionStatus(iocp_handle_, // IOCP 객체
				&dwIoSize,					// 실제로 전송된 바이트
				(PULONG_PTR)&P_client_info,		// CompletionKey
				&lpOverlapped,				// Overlapped IO 객체
				INFINITE);					// 대기할 시간

			//사용자 쓰레드 종료 메세지 처리..
			if (1 == bSuccess && 0 == dwIoSize && NULL == lpOverlapped)
			{
				is_worker_run_ = false;
				continue;
			}

			if (NULL == lpOverlapped)
			{
				continue;
			}

			auto P_overlapped_ex = (stOverlappedEx*)lpOverlapped;

			//client가 접속을 끊었을때..			
			if (0 == bSuccess || (0 == dwIoSize && IOOperation::ACCEPT != P_overlapped_ex->E_operation_))
			{
				//printf("socket(%d) 접속 끊김\n", (int)pClientInfo->_socketClient);
				CloseSocket(P_client_info);
				continue;
			}

			//Overlapped I/O Accept작업 결과 뒤 처리
			if (IOOperation::ACCEPT == P_overlapped_ex->E_operation_)
			{
				P_client_info = GetClientInfo(P_overlapped_ex->session_index_);
				if (P_client_info->AcceptCompletion())
				{
					// 클라이언트 갯수 증가
					++client_cnt_;

					OnConnect(P_client_info->GetIndex());
				}
				else {
					CloseSocket(P_client_info, true);
				}
			}

			//Overlapped I/O Recv작업 결과 뒤 처리
			else if (IOOperation::RECV == P_overlapped_ex->E_operation_)
			{
				// 클라이언트에서 받은 메세지를 PacketData 형태로 변환
				OnReceive(P_client_info->GetIndex(), dwIoSize, P_client_info->GetRecvBuffer());

				// Recv 완료 했기 때문에 재차 Recv() 호출
				P_client_info->BindRecv();
				//클라이언트에 메세지를 에코한다.

				//SendMsg(pClientInfo, pClientInfo->mRecvBuf, dwIoSize);
			}
			//Overlapped I/O Send작업 결과 뒤 처리
			else if (IOOperation::SEND == P_overlapped_ex->E_operation_)
			{
				P_client_info->SendCompleted(dwIoSize);

				//printf("[발신] bytes : %d , msg : %d\n", dwIoSize, pClientInfo->mSendBuf[0]);
				//printf("[발신] bytes : %d , msg : %s\n", dwIoSize, pClientInfo->mSendBuf);
			}
			//예외 상황
			else
			{
				//printf("socket(%d)에서 예외상황\n", (int)P_client_info->GetIndex());
				UE_LOG(LogTemp, Warning, TEXT("socket(%d) Exception\n"), (int)P_client_info->GetIndex());
			}
		}
	}

	//사용자의 접속을 받는 쓰레드
	void AccepterThread()
	{
		while (is_accepter_run_)
		{
			// 시간 처리
			auto cur_time_sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
			//접속을 받을 구조체의 인덱스를 얻어온다.

			// 연결이 되지 않은 객체가 있으면 client->ReadyAccept(listen_socket_, cur_time_sec);으로 
			// Acceptex 예약을 걸어둔다.
			for (auto client : client_Infos_)
			{
				if (client->IsConnected())
				{
					continue;
				}

				// 연결이 끊어진 클라의 경우 바로 연결하면 통신이 꼬일수도 있고 Wsasend가 발생할수도 있기 때문에
				// 일정 시간 기다리고 client->ReadyAccept(listen_socket_, cur_time_sec); AcceptEx 재 예약
				if ((UINT64)cur_time_sec < client->GetLatestClosedTimeSec())
				{
					continue;
				}

				// 연결된 클라이언트가 끊어졌는데 정해진 일정 시간을 초과 하지 않으면 일단 무시
				auto diff = cur_time_sec - client->GetLatestClosedTimeSec();
				if (diff <= RE_USE_SESSION_WAIT_TIMESEC)
				{
					continue;
				}

				// 연결이 되어 있지 않는 클라이언트가 있으면 비동기 accept 호출
				client->ReadyAccept(listen_socket_, cur_time_sec);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(32));
		}
	}


	//소켓의 연결을 종료 시킨다.
	void CloseSocket(ClientInfo* pClientInfo, bool bIsForce = false)
	{
		if (pClientInfo->IsConnected() == false)
		{
			return;
		}

		auto clientIndex = pClientInfo->GetIndex();

		pClientInfo->Close(bIsForce);

		OnClose(clientIndex);
	}

	// workerThread 갯수
	UINT32 max_io_worker_thread_count_ = 0;

	//클라이언트 정보 저장 구조체
	std::vector<ClientInfo*> client_Infos_;

	//클라이언트의 접속을 받기위한 리슨 소켓
	SOCKET		listen_socket_ = INVALID_SOCKET;

	//접속 되어있는 클라이언트 수
	int			client_cnt_ = 0;

	//작업 쓰레드 동작 플래그
	bool		is_worker_run_ = true;
	//IO Worker 스레드
	std::vector<std::thread> io_worker_threads_;

	//접속 쓰레드 동작 플래그
	bool		is_accepter_run_ = true;
	//Accept 스레드
	std::thread	accepter_thread_;

	//CompletionPort객체 핸들
	HANDLE		iocp_handle_ = INVALID_HANDLE_VALUE;
};