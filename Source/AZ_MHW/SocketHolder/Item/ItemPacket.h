#pragma once

struct CS_ITEM_TOTAL_INFO_REQ : public PACKET_HEADER
{
	int32 character_index;
	CS_ITEM_TOTAL_INFO_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_ITEM_TOTAL_INFO_REQ;
		packet_length = sizeof(CS_ITEM_TOTAL_INFO_REQ);
	}
};

struct SC_ITEM_TOTAL_INFO_RES : public PACKET_HEADER
{
	int32 character_index;
	char item_info[1024];
	SC_ITEM_TOTAL_INFO_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_ITEM_TOTAL_INFO_RES;
		packet_length = sizeof(SC_ITEM_TOTAL_INFO_RES);
	}
};

struct CS_ITEM_UNEQUIP_REQ : public PACKET_HEADER
{
	int32 character_index;
	int32 item_index;
	CS_ITEM_UNEQUIP_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_ITEM_UNEQUIP_REQ;
		packet_length = sizeof(CS_ITEM_UNEQUIP_REQ);
	}
};

struct SC_ITEM_UNEQUIP_RES : public PACKET_HEADER
{
	SC_ITEM_UNEQUIP_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_ITEM_UNEQUIP_RES;
		packet_length = sizeof(SC_ITEM_UNEQUIP_RES);
	}
};

struct CS_ITEM_EQUIP_REQ : public PACKET_HEADER
{
	int32 character_index;
	int32 item_index;
	CS_ITEM_EQUIP_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_LOGIN_SIGNIN_REQ;
		packet_length = sizeof(CS_ITEM_EQUIP_REQ);
	}
};

struct SC_ITEM_EQUIP_RES : public PACKET_HEADER
{
	SC_ITEM_EQUIP_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_ITEM_EQUIP_RES;
		packet_length = sizeof(SC_ITEM_EQUIP_RES);
	}
};

struct CS_ITEM_MOVE_REQ : public PACKET_HEADER
{
	int32 character_index;
	int32 item_index;
	short slot;
	CS_ITEM_MOVE_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_ITEM_MOVE_REQ;
		packet_length = sizeof(CS_ITEM_MOVE_REQ);
	}
};

struct SC_ITEM_MOVE_RES : public PACKET_HEADER
{
	SC_ITEM_MOVE_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_ITEM_MOVE_RES;
		packet_length = sizeof(SC_ITEM_MOVE_RES);
	}
};

struct CS_ITEM_CREATE_REQ : public PACKET_HEADER
{
	int32 character_index;
	int32 item_index;
	short slot;
	CS_ITEM_CREATE_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_ITEM_CREATE_REQ;
		packet_length = sizeof(CS_ITEM_CREATE_REQ);
	}
};

struct SC_ITEM_CREATE_RES : public PACKET_HEADER
{
	SC_ITEM_CREATE_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_ITEM_CREATE_RES;
		packet_length = sizeof(SC_ITEM_CREATE_RES);
	}
};

struct CS_ITEM_USE_REQ : public PACKET_HEADER
{
	int32 character_index;
	int32 item_index;
	CS_ITEM_USE_REQ()
	{
		packet_id = (UINT16)PACKET_ID::CS_ITEM_USE_REQ;
		packet_length = sizeof(CS_ITEM_USE_REQ);
	}
};

struct SC_ITEM_USE_RES : public PACKET_HEADER
{
	SC_ITEM_USE_RES()
	{
		packet_id = (UINT16)PACKET_ID::SC_ITEM_USE_RES;
		packet_length = sizeof(SC_ITEM_USE_RES);
	}
};

struct SC_ITEM_INFO_CMD : public PACKET_HEADER
{
	int32 character_index;
	int32 item_index;
	short slot;
	int32 count;
	SC_ITEM_INFO_CMD()
	{
		packet_id = (UINT16)PACKET_ID::SC_ITEM_INFO_CMD;
		packet_length = sizeof(SC_ITEM_INFO_CMD);
	}
};