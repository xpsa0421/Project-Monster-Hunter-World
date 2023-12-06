// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/ClientMsg/AZMsgHandler.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZ_MHW/Manager/AZGameConfig.h"
#include "AZ_MHW/Manager/AZGameOption.h"
#include "AZ_MHW/Manager/AZSaveData.h"
#include "AZ_MHW/HUD/AZHUDDataMgr.h"
#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/GameMode/AZGameMode_InGame.h"
#include "AZ_MHW/HUD/AZHUD_InGame.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/Manager/AZInventoryManager.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/Manager/SaveData/GameOptionSaveData.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
//FIXME merged need del
#include <GameFramework/Character.h>

//MinSuhong Add
#include "App_Server.h" // 서버 클래스 [ODBC 연결되어 있음 Packet.h]
//#include "Client_To_Server.h" // 서버 접속 클래스
#include "Odbc.h"
#include "TimerManager.h"
#include "UserManager.h"
#include "Manager/AZMonsterMgr.h"

UAZGameInstance::UAZGameInstance()
{

}

void UAZGameInstance::Init()
{
	Super::Init();

	UAZGameSingleton::instance();

	recv_buffer_offset_ = 0;
	memset(client_recv_buffer_, 0, sizeof(client_recv_buffer_));
	memset(client_recv_temp_buffer_, 0, sizeof(client_recv_temp_buffer_));

	send_buffer_offset_ = 0;
	memset(send_buffer_, 0, sizeof(send_buffer_));

	packet_function_ = NewObject<UPacketFunction>(this);
	packet_function_->Init();
	call_recv_packet_.BindUObject(packet_function_ ,&UPacketFunction::ProcessPacket);

	msg_handler = NewObject<UAZMsgHandler>(this);

	game_config = NewObject<UAZGameConfig>(this);
	game_config->Load();

	save_data = NewObject<UAZSaveData>(this);
	save_data->LoadAll();

	hud_data_mgr = NewObject<UAZHUDDataMgr>(this);
	hud_data_mgr->Init();

	game_option = NewObject<UAZGameOption>(this);
	game_option->Init();
	UAZGameOption::option_save_data = &save_data->option_save_data->game_option_save_data;

	login_mgr = NewObject<UAZLoginMgr>(this);
	login_mgr->Init();

	game_cache_info_ = NewObject<UAZGameCacheInfo>(this);
	game_cache_info_->Init();

	//inventory_mgr = NewObject<UAZInventoryManager>(this);
	//inventory_mgr->Init();
	
	input_mgr_ = NewObject<UAZInputMgr>(this);
	input_mgr_->Init();
	
	AddNewSingleton(map_mgr = NewObject<UAZMapMgr>(this));
	msg_handler->OnRegister(map_mgr);

	CreateSocketHolder();
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAZGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAZGameInstance::EndLoadingScreen);

	float TimerRate = 1.0f / 60.0f;  // 초당 60회
	//1player tick = 1초 60회 미만

	if (iocp_net_server_ == nullptr)
	{
		iocp_net_server_ = NewObject<UApp_Server>(this, TEXT("iocp_net_server_"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Start Server!"));
	// 서버 패킷 큐 타이머
	GetWorld()->GetTimerManager().SetTimer(server_timer_handle_, this, &UAZGameInstance::TimerProcessPacket, TimerRate, true);
	UE_LOG(LogTemp, Warning, TEXT("Start Client!"));
	// 클라이언트 패킷 큐 타이머
	GetWorld()->GetTimerManager().SetTimer(client_timer_handle_, this, &UAZGameInstance::ClientTimerProcessPacket, TimerRate, true);

	// 클라이언트 send 큐 타이머
	GetWorld()->GetTimerManager().SetTimer(client_send_timer_handle_, this, &UAZGameInstance::ClientSendProcess, TimerRate, true);
}

void UAZGameInstance::Shutdown()
{
	Super::Shutdown();

	UE_LOG(LogTemp, Warning, TEXT("Shutdown!"));

	if (iocp_net_server_->server_check_ == true)
	{
		iocp_net_server_->End();
	}

	DestroySocketHolder();
	//SendLogoutCmd();

	mgrs.Reset();
	AZ_LOG("Shutdown");

	// Destroy in reverse order of creation
	input_mgr_ = nullptr;
	game_option = nullptr;
	login_mgr = nullptr;
	save_data = nullptr;
	game_config = nullptr;
	map_mgr = nullptr;
	hud_data_mgr = nullptr;
	packet_function_ = nullptr;
}

void UAZGameInstance::RestMgr()
{
	for (auto* mgr : mgrs)
	{
		mgr->ResetMgr();
	}
	AZ_LOG("ResetMgr");
}

void UAZGameInstance::LobbyEnterAck()
{
	for (auto* mgr : mgrs)
	{
		mgr->LobbyEnterAck();
	}
	AZ_LOG("LobbyEnterAck");
}

void UAZGameInstance::PlayerSelectAck()
{
	for (auto* mgr : mgrs)
	{
		mgr->PlayerSelectAck();
	}
	AZ_LOG("PlayerSelectAck");
}

void UAZGameInstance::CreateSocketHolder()
{
	for (uint8 socket_type = 0; socket_type < (int32)ESocketHolderType::Max; ++socket_type)
	{
		UAZSocketHolder* socket_hodler = NewObject<UAZSocketHolder>(this);
		socket_hodler->Init((ESocketHolderType)socket_type);
		array_socket_holder_.Add(socket_hodler);
	}
}

void UAZGameInstance::DestroySocketHolder()
{
	for (UAZSocketHolder* socket_holder : array_socket_holder_)
	{
		if (socket_holder != nullptr)
		{
			socket_holder->Disconnect();
			socket_holder = nullptr;
		}
	}
}

void UAZGameInstance::InitSocketOnMapLoad()
{

}

UAZSocketHolder* UAZGameInstance::GetSocketHolder(ESocketHolderType socket_type)
{
	if (array_socket_holder_.Num() != (int32)ESocketHolderType::Max)
	{
		return nullptr;
	}

	if (socket_type >= ESocketHolderType::Max)
	{
		return nullptr;
	}

	if (GetGameMode() == nullptr)
	{
		return nullptr;
	}
	return array_socket_holder_[(int32)socket_type];
}

bool UAZGameInstance::IsWaitingProtocolEmpty()
{
	bool is_empty = true;
	for (auto socket_holder : array_socket_holder_)
	{
		if (socket_holder == nullptr)
		{
			continue;
		}

		if (socket_holder->IsWaitingProtocolEmpty() == false)
		{
			is_empty = false;
		}
	}
	return is_empty;
}

TArray<FString> UAZGameInstance::GetWaitingPorotocolNames() const
{
	TArray<FString> result_array;
	for (auto socket_holder : array_socket_holder_)
	{
		result_array.Append(socket_holder->GetWaitingProtocolNames());
	}

	return result_array;
}

void UAZGameInstance::BindLevelAddRemoveEvents()
{
	FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UAZGameInstance::OnLevelRemovedFromWorld);
	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UAZGameInstance::OnLevelAddedToWorld);
}

// TEMP: 메인메뉴 -> 인게임 로딩시 메인메뉴 레벨 사라지는 이벤트
void UAZGameInstance::OnLevelRemovedFromWorld(ULevel* in_level, UWorld* in_world)
{
	if (!in_level) // when transition starts
	{
		// 로딩 스크린 띄우기
		FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
	}
}

// TEMP: 스트리밍 레벨 (전투 맵) 추가되는 이벤트를 위해 사용
void UAZGameInstance::OnLevelAddedToWorld(ULevel* in_level, UWorld* in_world)
{
	// TODO 로딩 위젯 지우기
	FWorldDelegates::LevelAddedToWorld.RemoveAll(this);

	auto n = in_level->GetName();
	// TEMP LOAD FINISHED PACKET
	// CS_COMBAT_MAP_LOAD_FINISH_CMD packet;
	// GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
}

void UAZGameInstance::OnLevelLoaded()
{
	// TEMP JOIN PACKET
	CS_COMBAT_MAP_ENTER_REQ packet;
	GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
}

AAZHUD* UAZGameInstance::GetHUD()
{
	if (GetPlayerController() == nullptr)
	{
		return nullptr;
	}
	auto hud = Cast<AAZHUD>(GetPlayerController()->GetHUD());
	if (hud == nullptr)
	{
		return nullptr;
	}
	return hud;
}

AAZHUD_InGame* UAZGameInstance::GetHUDInGame()
{
	return GetPlayerController() ? Cast<AAZHUD_InGame>(GetPlayerController()->GetHUD()) : nullptr;
}

AAZGameMode* UAZGameInstance::GetGameMode()
{
	if (GetWorld() == nullptr)
	{
		return nullptr;
	}
	return GetWorld() ? Cast<AAZGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
}

AAZGameMode_InGame* UAZGameInstance::GetInGameMode()
{
	return Cast<AAZGameMode_InGame>(GetGameMode());
}

UAZGameState* UAZGameInstance::GetCurGameState()
{
	auto* game_mode = GetGameMode();
	if (game_mode == nullptr)
	{
		return nullptr;
	}
	return game_mode->GetCurGameState();
}

AAZPlayerController* UAZGameInstance::GetPlayerController()
{
	if (GetWorld() == nullptr)
	{
		return nullptr;
	}

	auto controller = Cast<AAZPlayerController>(GetFirstLocalPlayerController(GetWorld()));
	if (controller == nullptr)
	{
		return nullptr;
	}
	return controller;
}

AAZPlayer_Playable* UAZGameInstance::GetPlayer()
{
	auto* player_controller = GetPlayerController();
	if (player_controller == nullptr)
	{
		return nullptr;
	}
	//FIXME working check
	auto player = player_controller->GetPawn<AAZPlayer_Playable>();
	return player;
}

void UAZGameInstance::BeginLoadingScreen(const FString& map_name)
{
}

void UAZGameInstance::EndLoadingScreen(UWorld* in_loaded_world)
{
}

void UAZGameInstance::OnGameMsg(FAZGameMsg* game_msg)
{
}

void UAZGameInstance::SendLogoutCmd()
{
}

void UAZGameInstance::AddNewSingleton(UAZSingletonObject* mgr)
{
}

void UAZGameInstance::TimerProcessPacket()
{
	//UE_LOG(LogTemp, Warning, TEXT("TimerProcessPacket"));

	UINT32 user_index = 0;
	{
		std::lock_guard<std::mutex> grard(lock_);
		// 현재 Send 요청을 보낸 유저가 있는지 확인
		if (!server_in_coming_packet_user_index_.empty())
		{
			user_index = server_in_coming_packet_user_index_.front();

			// 알아낸 index로 유저 객체 가져옴
			while (true)
			{
				auto P_user = user_manager_->GetUserByConnIdx(user_index);
				auto packet_data = P_user->GetPacket();
				packet_data.client_index_ = user_index;

				if (packet_data.packet_id_ > (UINT16)PACKET_ID::SYS_END)
				{
					//is_idle = false;
					// 요청이 있는 경우 처리
					ProcessRecvPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
				}
				else
				{
					break;
				}
			}
			server_in_coming_packet_user_index_.pop_front();
		}
	}

	// 시스템 패킷 (연결 & 연결 종료가 발생한 경우)
	if (auto packet_data = DequeSystemPacketData(); packet_data.packet_id_ != 0)
	{
		//is_idle = false;
		// 요청이 있는 경우 처리
		ProcessSystemPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
	}

	if (timer_destroy_sw)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(server_timer_handle_);
	}
}

void UAZGameInstance::PacketInit(const UINT32 max_client)
{
	CreateComponent(max_client);
}

void UAZGameInstance::PacketEnd()
{
	is_run_process_thread_ = false;

	if (process_thread_.joinable())
	{
		process_thread_.join();
	}
}

/*void UAZGameInstance::DbRun()
{
	odbc->Init();
	odbc->ConnectMssql(L"odbc_test.dsn");
	odbc->Load();
}*/

void UAZGameInstance::ReceivePacketData(const UINT32 client_index, const UINT32 size, char* P_data)
{
	UE_LOG(LogTemp, Warning, TEXT("ReceivePacketData_GameInstance client : %d, size : %d"), client_index, size);
	// GetUserByConnIdx 어떤 클라이언트가 메세지를 보냈는지 확인
	auto P_user = user_manager_->GetUserByConnIdx(client_index);

	// 메세지를 보낸 클라이언트 객체에 데이터를 담음
	P_user->SetPacketData(size, P_data);

	// 큐에 리시브가 발생했다는 것을 알려줌
	EnqueuePacketData(client_index);
}

void UAZGameInstance::PushSystemPacket(PacketInfo packet)
{
	std::lock_guard<std::mutex> guard(lock_);
	// 패킷 헤더에 따라서 처리
	system_packet_queue_.push_back(packet);
}

SQLTCHAR* UAZGameInstance::ConvertCharToSqlTCHAR(const char* charArray)
{
	int charArrayLen = strlen(charArray) + 1;
	int numChars = MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, NULL, 0);

	SQLTCHAR* sqlTCharArray = new SQLTCHAR[numChars];

	MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, (LPWSTR)sqlTCharArray, numChars);

	return sqlTCharArray;
}

void UAZGameInstance::CreateComponent(const UINT32 max_client)
{
	user_manager_ = new UserManager;
	user_manager_->Init(max_client);
	odbc = new Odbc;
}

void UAZGameInstance::ClearConnectionInfo(INT32 client_index)
{
	auto P_req_user = user_manager_->GetUserByConnIdx(client_index);

	if (P_req_user->GetDomainState() != User::DOMAIN_STATE::NONE)
	{
		user_manager_->DeleteUserInfo(P_req_user);
	}
}

void UAZGameInstance::EnqueuePacketData(const UINT32 client_index)
{
	std::lock_guard<std::mutex> guard(lock_);
	server_in_coming_packet_user_index_.push_back(client_index);
}

PacketInfo UAZGameInstance::DequeSystemPacketData()
{
	std::lock_guard<std::mutex> guard(lock_);
	if (system_packet_queue_.empty())
	{
		return PacketInfo();
	}

	auto packet_data = system_packet_queue_.front();
	system_packet_queue_.pop_front();

	return packet_data;
}

void UAZGameInstance::ProcessSystemPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet)
{
	switch ((PACKET_ID)packet_id)
	{
	case PACKET_ID::SYS_USER_CONNECT:
	{
		ProcessuserConnect(client_index, packet_size, P_packet);
	}
	break;
	case PACKET_ID::SYS_USER_DISCONNECT:
	{
		ProcessuserConnect(client_index, packet_size, P_packet);
	}
	break;
	}
}

void UAZGameInstance::ProcessRecvPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet)
{
	if (call_recv_packet_.IsBound())
	{
		PACKET_HEADER* recv_packet = (PACKET_HEADER*)P_packet;
		call_recv_packet_.Execute(client_index, recv_packet);
	}
}

void UAZGameInstance::ProcessuserConnect(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	// 새로 연결 받기
	printf("[ProcessUserConnect_Gameinstance] clientIndex: %d\n", client_index);
	// 하나의 user_manger 객체 할당
	auto P_user = user_manager_->GetUserByConnIdx(client_index);
	// 객체 초기화
	P_user->Clear();
	game_cache_info_->client_index_ = client_index;
}

void UAZGameInstance::ProcessUserDisConnect(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	game_cache_info_->RemoveClientIndexToIdHashCode(client_index);
	printf("[ProcessUserDisConnect_Gameinstance] clientIndex: %d\n", client_index);
	// 연결이 끊어진 경우 user 반환
	ClearConnectionInfo(client_index);
}

// client
bool UAZGameInstance::Server_Connect(const FString& ip, int32 port)
{
	if (client_check == true)
	{
		return false;
	}

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(TCHAR_TO_ANSI(*ip));
	sa.sin_port = htons(port);

	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));

	if (iRet != 0)
	{
		closesocket(sock);
		sock = NULL;
		return false;
	}

	u_long mode = 1;

	ioctlsocket(sock, FIONBIO, &mode);

	recv_thread_ = std::thread(&UAZGameInstance::receive_thread, this);

	client_check = true;

	return true;
}

void UAZGameInstance::Server_Connect()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	/*----------------------
	SOCKET 생성
	-----------------------*/
	sock = socket(AF_INET, SOCK_STREAM, 0);

	short sData = 10000;
	short tData = 0x2710;
	short fData = 0x1027;

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(10000);

	/*----------------------
		SOCKET 연결
		-----------------------*/
	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));

	u_long iMode = 1;
	/*----------------------언리얼 엔진 vector deleting destructor'() 에러 원인이 뭐야 : End시 Thread에 join하고 끝내야함 안하면 이지랄남
	SOCKET 논블럭킹 설정  | ioctlsocket
	-----------------------*/
	ioctlsocket(sock, FIONBIO, &iMode);

	recv_thread_ = std::thread(&UAZGameInstance::receive_thread, this);

	client_check = true;
}

void UAZGameInstance::Client_Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT("[Client_Shutdown]\n"));
	recevie_connected = false;

	if (client_check == true)
	{
		recv_thread_.join();
		client_check = false;
	}

	closesocket(sock);
	sock = NULL;
	WSACleanup();
}

int UAZGameInstance::Server_Packet_Send(const char* packet, int packet_size)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server_Packet_Send] sendData : %hs size : %d\n"), packet, packet_size);

	int len = send(sock, packet, packet_size, 0);
	return len;
}

void UAZGameInstance::receive_thread()
{
	int result;

	while (recevie_connected)
	{
		result = recv(sock, client_recv_buffer_, sizeof(client_recv_buffer_), 0);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		int remain_data = result;
		if (result > 0)
		{
			// 현재 오프셋에서 결과 값만큼 메모리 카피
			memcpy(client_recv_temp_buffer_ + recv_buffer_offset_, client_recv_buffer_, result);
			while (remain_data > 0)
			{
				int header_check = remain_data + recv_buffer_offset_;
				if (header_check < sizeof(PACKET_HEADER))
				{
					recv_buffer_offset_ = header_check;
					break;
				}
				//헤더 체크
				PACKET_HEADER* header = (PACKET_HEADER*)(client_recv_temp_buffer_);
				UINT16 packet_length = header->packet_length;
				int total_length = recv_buffer_offset_ + remain_data;
				// 헤더 크기와 같거나 더 많이 온 경우(현재 length는 패킷 전체 구조체 크기를 담고 있다.)
				if (packet_length <= total_length)
				{
					char* char_packet = new char[packet_length];

					memcpy(char_packet, client_recv_temp_buffer_, header->packet_length);
					{
						std::lock_guard<std::mutex> lock(received_data_mutex);
						PACKET_HEADER* base_packet = (PACKET_HEADER*)char_packet;
						receive_data_queue_.push(base_packet);
					}

					//메모리 위치를 앞으로 옳김(최대한 안전하게 처리하자)
					remain_data = remain_data - packet_length;
					memmove(client_recv_temp_buffer_, client_recv_temp_buffer_ + packet_length, remain_data);
					recv_buffer_offset_ = 0;
				}
				else
				{
					recv_buffer_offset_ = remain_data;
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		else if (result == 0)
		{
			recevie_connected = false;
			break;
		}
		else
		{
			int error = WSAGetLastError();
			switch (error)
			{
			case WSAEWOULDBLOCK:
			case WSAEINTR:
				break;
			default:
				recevie_connected = false;
				break;
			}
		}
	}
}

void UAZGameInstance::ClientTimerProcessPacket()
{
	if (recevie_connected == false)
	{
		GetSocketHolder(ESocketHolderType::Game)->Disconnect();
	}

	// 대기열에 액세스하기 위한 잠금 획득
	std::lock_guard<std::mutex> lock(received_data_mutex);

	// 큐에 받은 데이터가 있는지 확인
	if (!receive_data_queue_.empty())
	{
		// 대기열에서 처음 받은 데이터 가져오기
		PACKET_HEADER* base_packet = receive_data_queue_.front();
		if (call_recv_packet_.IsBound())
		{
			bool retval = call_recv_packet_.Execute(0, base_packet);
			if (retval == false)
			{
				AZ_LOG("[client Packet process failed]:[id:%d]", base_packet->packet_id);
			}
		}
		receive_data_queue_.pop();
		delete[] (char*)base_packet;
	}

	if (server_client_check)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(client_timer_handle_);
	}
}

void UAZGameInstance::ClientSendProcess()
{
	if (server_client_check)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(client_send_timer_handle_);
	}

	if (send_buffer_offset_ == 0)
	{
		return;
	}

	int len = Server_Packet_Send(send_buffer_, send_buffer_offset_);
	if (len > 0)
	{
		send_buffer_offset_ -= len;
		memmove(send_buffer_, send_buffer_ + len, send_buffer_offset_);
		return;
	}
	else if (len -1)
	{
		int error = WSAGetLastError();
		switch (error)
		{
		case WSAEWOULDBLOCK:
		case WSAEINTR:
			break;
		default:
			recevie_connected = false;
			FTimerManager& timerManager = GetWorld()->GetTimerManager();
			timerManager.ClearTimer(client_send_timer_handle_);
			break;
		}
	}
}

void UAZGameInstance::InGameAccept()
{
	UE_LOG(LogTemp, Warning, TEXT("[InGameAccept]\n"));
}

// client end



