#pragma once
#include "Packet.h"

class User
{
	const UINT32 PACKET_DATA_BUFFER_SIZE = 65535;

public:
	enum class DOMAIN_STATE
	{
		NONE = 0,
		LOGIN = 1,
		ROOM = 2
	};

	User() = default;
	~User() = default;

	void Init(const INT32 index)
	{
		index_ = index;
		packet_data_buffer_ = new char[PACKET_DATA_BUFFER_SIZE];
	}

	void Clear()
	{
		room_index_ = -1;
		user_id_ = "";
		is_confirm_ = false;
		cur_domain_state_ = DOMAIN_STATE::NONE;

		packet_data_buffer_w_pos_ = 0;
		packet_data_buffer_r_pos_ = 0;
	}

	int SetLogin(char* user_id)
	{
		cur_domain_state_ = DOMAIN_STATE::LOGIN;
		user_id_ = user_id;

		return 0;
	}

	void EnterRoom(INT32 room_index)
	{
		room_index_ = room_index;
		cur_domain_state_ = DOMAIN_STATE::ROOM;
	}

	void SetDomainState(DOMAIN_STATE value) { cur_domain_state_ = value; }

	INT32 GetCurrentRoom()
	{
		return room_index_;
	}

	INT32 GetNetConnIdx()
	{
		return index_;
	}

	std::string GetUserId() const
	{
		return user_id_;
	}

	DOMAIN_STATE GetDomainState()
	{
		return cur_domain_state_;
	}

	void SetPacketData(const UINT32 data_size, char* P_data)
	{
		// 링버퍼
		if ((packet_data_buffer_w_pos_ + data_size) >= PACKET_DATA_BUFFER_SIZE)
		{
			auto re_main_data_size = packet_data_buffer_w_pos_ - packet_data_buffer_r_pos_;

			if (re_main_data_size > 0)
			{
				CopyMemory(&packet_data_buffer_[0], &packet_data_buffer_[packet_data_buffer_r_pos_], re_main_data_size);
				packet_data_buffer_w_pos_ = re_main_data_size;
			}
			else
			{
				packet_data_buffer_w_pos_ = 0;
			}
			packet_data_buffer_r_pos_ = 0;
		}

		CopyMemory(&packet_data_buffer_[packet_data_buffer_w_pos_], P_data, data_size);
		packet_data_buffer_w_pos_ += data_size;
	}

	PacketInfo GetPacket()
	{
		const int PACKET_SIZE_LENGTH = 2;
		const int PACKET_TYPE_LENGTH = 2;
		short packet_size = 0;

		UINT32 re_main_byte = packet_data_buffer_w_pos_ - packet_data_buffer_r_pos_;

		if (re_main_byte < PACKET_HEADER_LENGTH)
		{
			return PacketInfo();
		}

		auto P_header = (PACKET_HEADER*)&packet_data_buffer_[packet_data_buffer_r_pos_];

		if (P_header->packet_length > re_main_byte)
		{
			return PacketInfo();
		}

		PacketInfo packet_info;
		packet_info.packet_id_ = P_header->packet_id;
		packet_info.data_size_ = P_header->packet_length;
		packet_info.P_data_ptr_ = &packet_data_buffer_[packet_data_buffer_r_pos_];

		packet_data_buffer_r_pos_ += P_header->packet_length;

		return packet_info;
	}

private:
	INT32 index_ = -1;

	INT32 room_index_ = -1;

	std::string user_id_;
	bool is_confirm_ = false;
	std::string auth_token_;

	DOMAIN_STATE cur_domain_state_ = DOMAIN_STATE::NONE;

	UINT32 packet_data_buffer_w_pos_ = 0;
	UINT32 packet_data_buffer_r_pos_ = 0;
	char* packet_data_buffer_ = nullptr;
};

