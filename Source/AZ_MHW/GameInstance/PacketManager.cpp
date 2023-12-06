#include "PacketManager.h"
#include <utility>
#include <cstring>
#include "UserManager.h"
#include "CoreMinimal.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows/MinWindows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include <sqltypes.h>

#include "AZGameInstance.h"
#include "AZ_MHW/GameInstance/Packet.h"
#include "PlayerController/AZPlayerController_Server.h"

void PacketManager::Init(const UINT32 max_client)
{
	CreateCompent(max_client);
}

void PacketManager::CreateCompent(const UINT32 max_client)
{
	user_manager_ = new UserManager;
	user_manager_->Init(max_client);
}

bool PacketManager::Run()
{
	//이 부분을 패킷 처리 부분으로 이동 시킨다.
	is_run_process_thread_ = true;

	return true;
}

void PacketManager::End()
{
	is_run_process_thread_ = false;
}

void PacketManager::ReceivePacketData(const UINT32 client_index, const UINT32 size, char* P_data)
{
	// GetUserByConnIdx 어떤 클라이언트가 메세지를 보냈는지 확인
	auto P_user = user_manager_->GetUserByConnIdx(client_index);

	// 메세지를 보낸 클라이언트 객체에 데이터를 담음
	P_user->SetPacketData(size, P_data);

	// 큐에 리시브가 발생했다는 것을 알려줌
	EnqueuePacketData(client_index);
}

void PacketManager::EnqueuePacketData(const UINT32 client_index)
{
	std::lock_guard<std::mutex> guard(lock_);
	in_coming_packet_user_index_.push_back(client_index);
}

PacketInfo PacketManager::DequePacketData()
{
	UINT32 user_index = 0;

	{
		std::lock_guard<std::mutex> grard(lock_);
		// 현재 Send 요청을 보낸 유저가 있는지 확인
		if (in_coming_packet_user_index_.empty())
		{
			return PacketInfo();
		}

		// 요청한 데이터가 있다면 index 추출
		user_index = in_coming_packet_user_index_.front();
		in_coming_packet_user_index_.pop_front();
	}

	// 알아낸 index로 유저 객체 가져옴
	auto P_user = user_manager_->GetUserByConnIdx(user_index);
	auto packet_data = P_user->GetPacket();
	packet_data.client_index_ = user_index;

	return packet_data;
}

void PacketManager::PushSystemPacket(PacketInfo packet)
{
	std::lock_guard<std::mutex> guard(lock_);
	// 패킷 헤더에 따라서 처리
	system_packet_queue_.push_back(packet);
}

void PacketManager::DbRun()
{
	odbc.Init();
	odbc.ConnectMssql(L"odbc_test.dsn");
	odbc.Load();
}

SQLTCHAR* PacketManager::ConvertCharToSqlTCHAR(const char* charArray)
{
	int charArrayLen = strlen(charArray) + 1;
	int numChars = MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, NULL, 0);

	SQLTCHAR* sqlTCharArray = new SQLTCHAR[numChars];

	MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, (LPWSTR)sqlTCharArray, numChars);

	return sqlTCharArray;
}

PacketInfo PacketManager::DequeSystemPacketData()
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
