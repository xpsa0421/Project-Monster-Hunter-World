#pragma once

struct CS_CHAT_MSG_CMD : public PACKET_HEADER
{
	char nick_name[33];
	char msg[1000];
	CS_CHAT_MSG_CMD()
	{
		packet_id = (UINT16)PACKET_ID::CS_CHAT_MSG_CMD;
		packet_length = sizeof(CS_CHAT_MSG_CMD);
	}
};

struct SC_CHAT_MSG_CMD : public PACKET_HEADER
{
	char nick_name[33];
	char msg[1000];
	SC_CHAT_MSG_CMD()
	{
		packet_id = (UINT16)PACKET_ID::SC_CHAT_MSG_CMD;
		packet_length = sizeof(SC_CHAT_MSG_CMD);
	}
};