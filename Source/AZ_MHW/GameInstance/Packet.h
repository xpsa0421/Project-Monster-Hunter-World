#pragma once

#include "AZWindows.h"
#include "Odbc.h"
#include "CommonPacket.h"

const UINT32 PACKET_HEADER_LENGTH = sizeof(PACKET_HEADER);

struct PacketInfo
{
	UINT32 client_index_ = 0;
	UINT16 packet_id_ = 0;
	UINT16 data_size_ = 0;
	char* P_data_ptr_ = nullptr;
};

