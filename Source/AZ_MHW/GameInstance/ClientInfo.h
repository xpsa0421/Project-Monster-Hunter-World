#pragma once
#include "Define.h"
#include "AZWindows.h"

const int RECV_BUFFER_SIZE = 20000;
class ClientInfo
{
public:
	ClientInfo()
	{
		ZeroMemory(&recv_overlapped_ex_, sizeof(stOverlappedEx));
		socket_ = INVALID_SOCKET;
	}

	void Init(const UINT32 index, HANDLE iocp_handle)
	{
		index_ = index;
		iocp_handle_ = iocp_handle;
	}

	UINT32 GetIndex() { return index_; }

	bool IsConnected() { return is_connect_ == 1; }

	SOCKET GetSock() { return socket_; }

	UINT64 GetLatestClosedTimeSec() { return latest_closed_time_sec_; }

	char* GetRecvBuffer() { return recv_buf_; }

	bool OnConnect(HANDLE iocpHandle, SOCKET socket)
	{
		socket_ = socket;
		is_connect_ = 1;

		Clear();

		// 클라이언트가 접속하면 listen_socket_ -> ReadyAccept()생성 Socket으로 변경
		if (SetIocpBindSocket(iocpHandle) == false)
		{
			return false;
		}

		return BindRecv();
	}

	void Close(bool bIsForce = false)
	{
		struct linger stLinger = { 0,0 }; // SO_DONTLINGER로 설정

		// bIsForce가 true이면 SO_LINGER, timeout = 0으로 설정하여 강제 종료 시킨다. 주의 : 데이터 손실이 있을수 있음 
		if (true == bIsForce)
		{
			stLinger.l_onoff = 1;
		}

		//socketClose소켓의 데이터 송수신을 모두 중단 시킨다.
		shutdown(socket_, SD_BOTH);

		//소켓 옵션을 설정한다.
		setsockopt(socket_, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

		is_connect_ = 0;
		latest_closed_time_sec_ = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		//소켓 연결을 종료 시킨다. 
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}

	void Clear()
	{
	}

	// 비동기 accept 연결 예약
	bool ReadyAccept(SOCKET listen_socket, const UINT64 cur_time_sec)
	{
		//printf_s("ReadyAccept. client Index: %d\n", GetIndex());
		UE_LOG(LogTemp, Warning, TEXT("ReadyAccept. client Index: %d\n"), GetIndex());

		latest_closed_time_sec_ = UINT32_MAX;

		// socket_ = INVALID_SOCKET 에서 인터넷 가능 소켓을 할당
		socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == socket_)
		{
			//printf_s("client Socket WSASocket Error : %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("client Socket WSASocket Error : %d\n"), GetLastError());
			return false;
		}

		ZeroMemory(&accept_overlapped_ex_, sizeof(stOverlappedEx));

		DWORD bytes = 0;
		DWORD flags = 0;
		accept_overlapped_ex_.wsa_buf_.len = 0;
		accept_overlapped_ex_.wsa_buf_.buf = nullptr;
		accept_overlapped_ex_.E_operation_ = IOOperation::ACCEPT;
		accept_overlapped_ex_.session_index_ = index_;

		// AcceptEx : 비동기 accept
		// https://snowfleur.tistory.com/116
		int temp = 0;
		if (temp == AcceptEx(
			listen_socket, // 서버 애플리케이션이 이 소켓에서 연결을 시도 할때까지 기다림
			socket_, // 들어오는 연결을 수락할 소켓
			accept_buf_,  // 새 연결에서 보낸 첫 번째 데이터 블록, 서버의 로컬 주소 및 클라이언트의 원격 주소를 수신하는 버퍼에 대한 포인터
			0, // 0이면 수신 작업이 발생하지 않고 연결 하는 즉시 완료
			sizeof(SOCKADDR_IN) + 16, // 로컬 정보 주소 위한 예약된 바이트 수 전송 프로토콜보다 16 더 길어야함
			sizeof(SOCKADDR_IN) + 16,  // 원격 주소 정보를 위한 예약된 바이트 수 전송 프로토콜보다 16 더 길어야함
			&bytes, (LPWSAOVERLAPPED) & (accept_overlapped_ex_))) // 요청처리를 사용되는 OVERLAPPED 구조체 (NULL일수가 없음)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				//printf_s("AcceptEx Error : %d\n", GetLastError());
				UE_LOG(LogTemp, Warning, TEXT("AcceptEx Error : %d\n"), GetLastError());
				return false;
			}
		}

		return true;
	}

	// accept 완료 함수
	bool AcceptCompletion()
	{
		//printf_s("AcceptCompletion : SessionIndex(%d)\n", index_);
		UE_LOG(LogTemp, Warning, TEXT("AcceptCompletion : SessionIndex(%d)\n"), index_);

		if (OnConnect(iocp_handle_, socket_) == false)
		{
			return false;
		}

		SOCKADDR_IN st_client_addr;
		int addr_len = sizeof(SOCKADDR_IN);
		char client_ip[32] = { 0, };
		inet_ntop(AF_INET, &(st_client_addr.sin_addr), client_ip, 32 - 1);
		//printf("클라이언트 접속 : IP(%s) SOCKET(%d)\n", client_ip, (int)socket_);
		UE_LOG(LogTemp, Warning, TEXT("클라이언트 접속 : IP(%s) SOCKET(%d)\n"), client_ip, (int)socket_);

		return true;
	}

	bool SetIocpBindSocket(HANDLE iocpHandle)
	{
		// listen_socket_ -> ReadyAccept()에서 생성한 소켓으로 변경
		auto H_iocp = CreateIoCompletionPort((HANDLE)GetSock(),
			iocpHandle, // ExistingCompletionPort : 파일 또는 소켓과 연결할 입출력 완료 포트
			(ULONG_PTR)(this), // CompletionKey로 어떤 소켓이 I/O를 했는지 알 수 있음
			0);

		if (H_iocp == INVALID_HANDLE_VALUE)
		{
			//printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[에러] CreateIoCompletionPort()함수 실패: %d\n"), GetLastError());
			return false;
		}
		return true;
	}

	bool BindRecv()
	{
		DWORD dwFlag = 0;
		DWORD dwRecvNumBytes = 0;

		recv_overlapped_ex_.wsa_buf_.len = RECV_BUFFER_SIZE;
		recv_overlapped_ex_.wsa_buf_.buf = recv_buf_;
		recv_overlapped_ex_.E_operation_ = IOOperation::RECV;

		int nRet = WSARecv(socket_,
			&(recv_overlapped_ex_.wsa_buf_),
			1, &dwRecvNumBytes, &dwFlag,
			(LPWSAOVERLAPPED) & (recv_overlapped_ex_),
			NULL);

		// socket_error이면 client socket이 끊어진걸로 처리한다.
		if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			//printf("[에러] WSARecv()함수 실패 : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[에러] WSARecv()함수 실패 : %d\n"), WSAGetLastError());
			return false;
		}

		return true;
	}

	bool SendMsg(const UINT32 data_size_, char* P_msg_)
	{
		Defind defind;
		auto P_login_req_packet = reinterpret_cast<Test_Login_Send_Packet*>(P_msg_);
		UE_LOG(LogTemp, Warning, TEXT("[SendMsg] packet_id : %d, packet_length : %d packet_data %s\n"),
			P_login_req_packet->packet_id, P_login_req_packet->packet_length, *defind.CharArrayToFString(P_login_req_packet->user_id));
		P_login_req_packet->user_id;
		// send_overlapped 구조체 생성
		auto send_overlapped_ex = new stOverlappedEx;
		ZeroMemory(send_overlapped_ex, sizeof(stOverlappedEx));
		send_overlapped_ex->wsa_buf_.len = data_size_;
		// Send 보낼 데이터 사이즈만큼 buf 동적 할당
		send_overlapped_ex->wsa_buf_.buf = new char[data_size_];
		CopyMemory(send_overlapped_ex->wsa_buf_.buf, P_msg_, data_size_);
		// send_overlapped 형태 알려주기 
		send_overlapped_ex->E_operation_ = IOOperation::SEND;

		std::lock_guard<std::mutex> guard(send_lock_);

		// send queue에 데이터 밀어 넣기
		send_data_queue_.push(send_overlapped_ex);

		// 현재 큐에 한개 밖에 없으면 실행해도 되고 2개 이상이면 현재 진행중인 WSASend가 있다
		if (send_data_queue_.size() == 1)
		{
			SendIO();
		}

		return true;
	}

	void SendCompleted(const UINT32 dataSize_)
	{
		//printf("[송신 완료] bytes : %d\n", dataSize_);
		UE_LOG(LogTemp, Warning, TEXT("[송신 완료] bytes : %d\n"), dataSize_);

		std::lock_guard<std::mutex> guard(send_lock_);

		// 큐의 제일 앞 배열 삭제
		delete[] send_data_queue_.front()->wsa_buf_.buf;
		// 큐 제일 앞 삭제
		delete send_data_queue_.front();

		// 큐 제일 앞 빼버리기
		send_data_queue_.pop();

		// 큐가 비어 있는 상태가 아니면 그 다음부터 다시 wsasend
		if (send_data_queue_.empty() == false)
		{
			SendIO();
		}
	}

private:
	bool SendIO()
	{
		// 큐의 제일 앞부분 stOverlappedEx로 객체에 담기
		auto send_overlapped_ex = send_data_queue_.front();

		DWORD dwRecvNumBytes = 0;
		int nRet = WSASend(
			socket_, // 비동기 입출력 소켓
			&(send_overlapped_ex->wsa_buf_), // WSABUF 구조체 배열 주소
			1, // WSABUF 구조체 갯수
			&dwRecvNumBytes, // 함수 호출이 성공하면 이 변수에 보내거나 받은 바이트 수가 저장
			0, // send()와 recv()함수의 마지막 인자와 동일한 역할
			(LPWSAOVERLAPPED)send_overlapped_ex, // WSAOVERLAPPED 구조체 변수 주소값
			NULL); // 입출력 작업이 완료되면 운영체제가 자동으로 호출할 완료루틴 주소 값

		//socket_error이면 client socket이 끊어진걸로 처리한다.
		if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			//printf("[에러] WSASend()함수 실패 : %d\n", WSAGetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[에러] WSASend()함수 실패 : %d\n"), WSAGetLastError());
			return false;
		}
		return true;
	} // SendIO End

	bool SetSocketOption()
	{
		int opt = 1;
		if (SOCKET_ERROR == setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt,
			sizeof(int)))
		{
			//printf_s("[DEBUG] TCP_NODELAY error: %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[DEBUG] TCP_NODELAY error : % d\n"), GetLastError());
			return false;
		}

		opt = 0;
		if (SOCKET_ERROR == setsockopt(socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
		{
			//printf_s("[DEBUG] SO_RCVBUF change error: %d\n", GetLastError());
			UE_LOG(LogTemp, Warning, TEXT("[DEBUG] SO_RCVBUF change error: %d\n"), GetLastError());
			return false;
		}

		return true;
	}

	INT32 index_ = 0;
	HANDLE iocp_handle_ = INVALID_HANDLE_VALUE;

	INT64 is_connect_ = 0;
	UINT64 latest_closed_time_sec_ = 0;


	SOCKET socket_; //Cliet와 연결되는 소켓

	// Accept
	stOverlappedEx accept_overlapped_ex_;
	char accept_buf_[64];

	// Recv
	stOverlappedEx recv_overlapped_ex_; //RECV Overlapped I/O작업을 위한 변수
	char recv_buf_[RECV_BUFFER_SIZE];  //데이터 버퍼

	// Send (queue 형태)
	std::mutex send_lock_;
	std::queue<stOverlappedEx*> send_data_queue_;
};

