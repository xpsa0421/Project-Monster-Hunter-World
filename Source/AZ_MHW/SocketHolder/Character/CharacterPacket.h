//guid번호는 SC에서만 유효하도록
#pragma once
#include "AZ_MHW/CommonSource/AZStruct.h"

// 전체 캐릭터 정보 얻기
struct CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ : public PACKET_HEADER
{
	CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ()
	{
		packet_length = sizeof(CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ);
		packet_id = (UINT16)(PACKET_ID::CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ);
	}
};

// 전체 캐릭터 정보 얻기 응답
struct SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES : public PACKET_HEADER
{
	int count = 0;
	FCharacterSimpleInfo info_0;
	FCharacterSimpleInfo info_1;
	FCharacterSimpleInfo info_2;

	SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES()
	{
		packet_length = sizeof(SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES);
		packet_id = (UINT16)(PACKET_ID::SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES);
	}
};

// 캐릭터 생성 창 캐릭터 생성
struct CS_PLAYER_CHARACTER_CREATE_REQ : public PACKET_HEADER
{
	FCharacterSimpleInfo create_info;
	CS_PLAYER_CHARACTER_CREATE_REQ()
	{
		packet_length = sizeof(CS_PLAYER_CHARACTER_CREATE_REQ);
		packet_id = (UINT16)(PACKET_ID::CS_PLAYER_CHARACTER_CREATE_REQ);
	}
};

struct SC_PLAYER_CHARACTER_CREATE_RES : public PACKET_HEADER
{
	FCharacterSimpleInfo create_info;
	SC_PLAYER_CHARACTER_CREATE_RES()
	{
		packet_length = sizeof(SC_PLAYER_CHARACTER_CREATE_RES);
		packet_id = (UINT16)(PACKET_ID::SC_PLAYER_CHARACTER_CREATE_RES);
	}
};

//캐릭터 생성패킷
struct CREATE_PLAYER_CHARACTER_PACKET : public PACKET_HEADER
{
	int32 guid;
	int32 character_index;
	
	CREATE_PLAYER_CHARACTER_PACKET()
	{
		packet_length = sizeof(CREATE_PLAYER_CHARACTER_PACKET);
		guid = -1;
	}
};
//캐릭터 제거패킷
struct DESTROY_PLAYER_CHARACTER_PACKET : public PACKET_HEADER
{
	int32 guid;
	int32 character_index;

	DESTROY_PLAYER_CHARACTER_PACKET()
	{
		packet_length = sizeof(DESTROY_PLAYER_CHARACTER_PACKET);
		guid = -1;
	}
};

//캐릭터 액션패킷
struct ACTION_PLAYER_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	FVector current_position;
	float current_direction;
	float input_direction;
	
	int32 input_data;

	ACTION_PLAYER_PACKET()
	{
		packet_length = sizeof(ACTION_PLAYER_PACKET);
		guid = -1;
		current_position = FVector::Zero();
		current_direction = 0.f;
		input_direction = 0.f;
		input_data = 0;
	}
};
//캐릭터 상태 갱신패킷
struct UPDATE_PLAYER_STATE_PACKET : public PACKET_HEADER
{
	int32 guid;
	/** 상태타입 변경*/
	int32 state_type;
	int32 state_value;
	/** 상태 비트마스크*/
	int32 state_bitmask;
	
	UPDATE_PLAYER_STATE_PACKET()
	{
		packet_length = sizeof(UPDATE_PLAYER_STATE_PACKET);
		guid = -1;
		state_type = -1;
		state_value = -1;
		
		state_bitmask = -1;
	}
};
//캐릭터 장비변경패킷
struct EQUIPMENT_PLAYER_PACKET : public PACKET_HEADER
{
	int32 guid;
	int32 item_id;

	EQUIPMENT_PLAYER_PACKET()
	{
		packet_length = sizeof(EQUIPMENT_PLAYER_PACKET);
		guid = -1;
		item_id = -1;
	}
};

//초기 상태초기전달용 패킷
struct INITIALIZE_PLAYER_STATE_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	FVector pos;
	float dir;
	
	int32 head_id;
	int32 hair_id;
	FVector4f hair_color;
	int32 body_id;
	int32 waist_id;
	int32 arm_id;
	int32 leg_id;
	int32 weapon_id;
	int32 first_id;
	int32 second_id;
	
	INITIALIZE_PLAYER_STATE_PACKET()
	{
		packet_length = sizeof(INITIALIZE_PLAYER_STATE_PACKET);
		guid = -1;
		
		pos = FVector::ZeroVector;
		dir = 0;

		hair_color = FVector4f::Zero();
		head_id = 0;
		hair_id=0;
		body_id=0;
		waist_id=0;
		arm_id=0;
		leg_id=0;
		
		weapon_id=0;
		first_id=0;
		second_id=0;
	}
};
//SC전용//DB처리를 클라에서 하면
struct INVENTORY_PLAYER_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	int32 item_id;
	int32 item_count;
	int32 item_pos;
	//0 == warehouse
	//1 == inventory
	//2 == pocket
	
	INVENTORY_PLAYER_PACKET()
	{
		packet_length = sizeof(INVENTORY_PLAYER_PACKET);
		guid = -1;
		
		item_id = -1;
		item_count = -1;
		item_pos = -1;
	}
};
struct GESTURE_PLAYER_PACKET : public PACKET_HEADER
{
	int32 guid;
	int32 gesture_id;

	GESTURE_PLAYER_PACKET()
	{
		packet_length = sizeof(GESTURE_PLAYER_PACKET);
		guid = -1;
		gesture_id = -1;
	}
};

struct HIT_PLAYER_PACKET :public PACKET_HEADER
{
	int32 guid;
	float hit_angle;
	int32 damage;
	
	HIT_PLAYER_PACKET()
	{
		packet_length = sizeof(HIT_PLAYER_PACKET);
		guid = -1;
		hit_angle = 0.f;
		damage = 1;
	}
};