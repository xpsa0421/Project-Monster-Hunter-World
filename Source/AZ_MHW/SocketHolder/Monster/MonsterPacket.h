#pragma once
#include "AZ_MHW/CommonSource/AZStruct.h"

// Monster base packet
struct FMonsterBasePacket : PACKET_HEADER
{
	int32 object_serial;

	FMonsterBasePacket() : object_serial(-1) {};
	FMonsterBasePacket(int32 serial) : object_serial(serial) {};
};

// S->C Packet to spawn a monster at a location
struct SC_MONSTER_SPAWN_CMD : FMonsterBasePacket
{
	int32 monster_id;
	EBossRank rank;
	FVector location;
	FRotator rotation;

	SC_MONSTER_SPAWN_CMD()
	{
		monster_id = -1;
		rank = EBossRank::None;
		location = FVector::ZeroVector;
		rotation = FRotator::ZeroRotator;
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_SPAWN_CMD);
		packet_length = sizeof(SC_MONSTER_SPAWN_CMD);
	};
};

// S->C Packet to notify end of monster spawn
struct SC_MONSTER_SPAWN_END_CMD : FMonsterBasePacket
{
	SC_MONSTER_SPAWN_END_CMD()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_SPAWN_END_CMD);
		packet_length = sizeof(SC_MONSTER_SPAWN_END_CMD);
	};
};


// S->C Packet to adjust the basic transform of a monster
// is_forced: true when used for initialisation
struct SC_MONSTER_TRANSFORM_CMD : FMonsterBasePacket
{
	FVector location;
	FRotator rotation;
	bool is_forced;

	SC_MONSTER_TRANSFORM_CMD() 
	{
		location = FVector::ZeroVector;
		rotation = FRotator::ZeroRotator;
		is_forced = true;
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_TRANSFORM_CMD);
		packet_length = sizeof(SC_MONSTER_TRANSFORM_CMD);
	};
};

// S->C Packet to initialise the states (part break, wound, sever) of body parts
// body_states map size is fixed to 
struct SC_MONSTER_BODY_STATE_CMD : FMonsterBasePacket
{
	FBossBodyPartDebuffState head_state;
	FBossBodyPartDebuffState back_state;
	FBossBodyPartDebuffState left_wing_state;
	FBossBodyPartDebuffState right_wing_state;
	FBossBodyPartDebuffState tail_state;

	SC_MONSTER_BODY_STATE_CMD()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_BODY_STATE_CMD);
		packet_length = sizeof(SC_MONSTER_BODY_STATE_CMD);
	};
};

// S->C Packet to indicate enter combat
struct SC_MONSTER_ENTER_COMBAT_CMD : FMonsterBasePacket
{
	SC_MONSTER_ENTER_COMBAT_CMD()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_ENTER_COMBAT_CMD);
		packet_length = sizeof(SC_MONSTER_ENTER_COMBAT_CMD);
	};
};

struct CS_MONSTER_UPDATE_REQ : FMonsterBasePacket
{
	CS_MONSTER_UPDATE_REQ()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::CS_MONSTER_UPDATE_REQ);
		packet_length = sizeof(CS_MONSTER_UPDATE_REQ);
	};
};

// S->C Packet to inform the start of new action of a monster
struct SC_MONSTER_ACTION_START_CMD : FMonsterBasePacket
{
	MonsterActionStateInfo state_info;
	float start_position;

	SC_MONSTER_ACTION_START_CMD()
	{
		start_position = 0.0f;
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_ACTION_START_CMD);
		packet_length = sizeof(SC_MONSTER_ACTION_START_CMD);
	};
};

struct SC_MONSTER_ACTION_END_CMD : FMonsterBasePacket
{
	EMonsterActionMode action_mode;
	FVector location;
	FRotator rotation;

	SC_MONSTER_ACTION_END_CMD()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_ACTION_END_CMD);
		packet_length = sizeof(SC_MONSTER_ACTION_END_CMD);
	};
};

// S->C Packet to inform the state change (part break, wound, sever) of body parts
struct SC_MONSTER_PART_CHANGE_CMD : FMonsterBasePacket
{
	EMonsterBodyPart body_part;
	EMonsterBodyPartChangeType change_type;

	SC_MONSTER_PART_CHANGE_CMD()
	{
		body_part = EMonsterBodyPart::Default;
		change_type = EMonsterBodyPartChangeType::Wound;
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_PART_CHANGE_CMD);
		packet_length = sizeof(SC_MONSTER_PART_CHANGE_CMD);
	};
};

// S->C Packet to inform enrage begin event
struct SC_MONSTER_ENRAGE_BEGIN_CMD : FMonsterBasePacket
{
	SC_MONSTER_ENRAGE_BEGIN_CMD()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_ENRAGE_BEGIN_CMD);
		packet_length = sizeof(SC_MONSTER_ENRAGE_BEGIN_CMD);
	};
};

// S->C Packet to inform enrage end event
struct SC_MONSTER_ENRAGE_END_CMD : FMonsterBasePacket
{
	SC_MONSTER_ENRAGE_END_CMD()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_ENRAGE_END_CMD);
		packet_length = sizeof(SC_MONSTER_ENRAGE_END_CMD);
	};
};

// S->C Packet to inform the success of player's attack to a monster
struct SC_MONSTER_HIT_CMD : FMonsterBasePacket
{
	FHitResultInfo hit_info;

	SC_MONSTER_HIT_CMD()
	{
		hit_info = FHitResultInfo();
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_HIT_CMD);
		packet_length = sizeof(SC_MONSTER_HIT_CMD);
	};
};

struct SC_MONSTER_DIE_CMD : FMonsterBasePacket
{
	SC_MONSTER_DIE_CMD()
	{
		packet_id = static_cast<UINT16>(PACKET_ID::SC_MONSTER_DIE_CMD);
		packet_length = sizeof(SC_MONSTER_DIE_CMD);
	};
};