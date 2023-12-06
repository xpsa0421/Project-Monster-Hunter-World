#pragma once

#include "Packet.h"

#include <unordered_map>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>

class UserManager;
class RoomManger;
class RedisManager;

class PacketManager
{
public:
	PacketManager() = default;
	~PacketManager() = default;

	void Init(const UINT32 max_client);

	bool Run();

	void End();

	void ReceivePacketData(const UINT32 client_index, const UINT32 size, char* P_data);

	void PushSystemPacket(PacketInfo packet);

	void DbRun();

	std::function<void(UINT32, UINT32, char*)> SendPacketFunc;
	std::function<void(UINT32, UINT32, char*)> BroadCastSendPacketFunc;

	SQLTCHAR* ConvertCharToSqlTCHAR(const char* charArray);

public:
	Odbc odbc;

private:
	void CreateCompent(const UINT32 max_client);

	void EnqueuePacketData(const UINT32 client_index);

	PacketInfo DequePacketData();

	PacketInfo DequeSystemPacketData();

	UserManager* user_manager_;

	std::function<void(int, char*)> send_mq_data_func_;

	bool is_run_process_thread_ = false;

	std::mutex lock_;

	// 실제 데이터가 왔을때 사용 하는 큐
	std::deque<UINT32> in_coming_packet_user_index_;

	// 네트워크 연결 & 끊어짐을 처리하는 큐
	std::deque<PacketInfo> system_packet_queue_;
};