// Copyright Team AZ. All Rights Reserved.


#include "App_Server.h"
#include "AZGameInstance.h"

UApp_Server::UApp_Server()
{
}

void UApp_Server::OnConnect(const UINT32 client_index)
{
	UE_LOG(LogTemp, Warning, TEXT("[OnConnect_App_Server] Client : Index(%d)\n"), client_index);
	PacketInfo packet{ client_index, (UINT16)PACKET_ID::SYS_USER_CONNECT, 0 };

	// SYS_USER_CONNECT 패킷을 전달
	P_packet_manager_->PushSystemPacket(packet);
}

void UApp_Server::OnClose(const UINT32 client_index_)
{
	UE_LOG(LogTemp, Warning, TEXT("[OnClose_App_Server] Client : Index(%d)\n"), client_index_);

	PacketInfo packet{ client_index_, (UINT16)PACKET_ID::SYS_USER_DISCONNECT, 0 };
	P_packet_manager_->PushSystemPacket(packet);
}

void UApp_Server::OnReceive(const UINT32 client_index, const UINT32 size, char* P_recv_data)
{
	UE_LOG(LogTemp, Warning, TEXT("[OnReceive_App_Server] Client : Index(%d), dataSize(%d), recvData : %hs\n"), client_index, size, P_recv_data);

	P_packet_manager_->ReceivePacketData(client_index, size, P_recv_data);
}

void UApp_Server::Run(const UINT32 max_client)
{
	// 람다로 funtion 객체에 SendMsg 담음
	auto send_packet_func = [&](UINT32 client_index, UINT16 packet_size, char* P_send_packet)
	{
		SendMsg(client_index, packet_size, P_send_packet);
	};

	auto boradcast_send_packet_func = [&](UINT32 client_index, UINT16 packet_size, char* P_send_packet)
	{
		BroadcastSendMsg(client_index, packet_size, P_send_packet);
	};

	// 패킷 매니저 생성
	//P_packet_manager_ = std::make_unique<PacketManager>();
	if (P_packet_manager_ == nullptr)
	{
		P_packet_manager_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	}
	P_packet_manager_->SendPacketFunc = send_packet_func;
	P_packet_manager_->BroadCastSendPacketFunc = boradcast_send_packet_func;

	// 클라이언트 수 만큼 user 생성 index, packet_data_buffer 초기화
	P_packet_manager_->PacketInit(max_client);

	// DB 접속
	//P_packet_manager_->DbRun();

	StartServer(max_client);
}

void UApp_Server::End()
{
	P_packet_manager_->PacketEnd();

	DestroyThread();
}

void UApp_Server::ServerStart()
{
	server_check_ = true;

	Init(MAX_IO_WORKER_THREAD2);

	//소켓과 서버 주소를 연결하고 등록 시킨다.
	BindAndListen(SERVER_PORT2);

	Run(MAX_CLIENT2);
}
