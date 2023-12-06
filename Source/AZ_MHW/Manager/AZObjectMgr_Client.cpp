// Copyright Team AZ. All Rights Reserved.


#include "AZObjectMgr_Client.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "CharacterComponent/AZMonsterPacketHandlerComponent_Client.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_InGame.h"
#include "GameSingleton/AZGameSingleton.h"

UAZObjectMgr_Client::UAZObjectMgr_Client()
{
}

void UAZObjectMgr_Client::BeginDestroy()
{
	UObject::BeginDestroy();
	object_map_.Empty();
}

bool UAZObjectMgr_Client::AddObject(int32 object_serial, AAZCharacter* character)
{
	if (!character) return false;
	
	if (object_map_.Find(object_serial))
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Client] Serial No: %d already exists!"), object_serial);
		return false;
	}
	else
	{
		object_map_.Add(object_serial, character);
		character->object_serial_ = object_serial;
		UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Client] %s spawn succeeded. Serial No: %d"), *character->GetName(), object_serial);
		return true;
	}
}

bool UAZObjectMgr_Client::RemoveObject(int32 object_serial)
{
	if (object_map_.Remove(object_serial))
	{
		UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Client] Serial No: %d is removed"), object_serial);
		return true;
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Client] Serial No: %d does not exist!"), object_serial);
		return false;
	}
}

bool UAZObjectMgr_Client::SpawnMonster(int32 object_serial, int32 monster_id, EBossRank rank, FVector spawn_location, FRotator spawn_rotation)
{
	// Retrieve the blueprint path of the monster to spawn
	const FName monster_name = UAZGameSingleton::instance()->monster_mgr_->GetMonsterName(monster_id);
	if (monster_name == NAME_None) return false;
	const FString bp_path = FString::Printf(TEXT("Blueprint'/Game/AZ/Monsters/%s/Blueprints/BP_%s_Client.BP_%s_Client'"),
		*monster_name.ToString(), *monster_name.ToString(), *monster_name.ToString());
	
	// Spawn a monster from the blueprint with the received transform
	const UBlueprint* monster_bp = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), this, *bp_path));
	if (UWorld* world = GetOuter()->GetWorld())
	{
		const FTransform spawn_transform(spawn_rotation, spawn_location);
		AAZMonster_Client* monster = world->SpawnActorDeferred<AAZMonster_Client>(monster_bp->GeneratedClass, spawn_transform);
		monster->Init(monster_id, rank);
		monster->FinishSpawning(spawn_transform);

		if (AddObject(object_serial, monster))
		{
			monster->packet_handler_component_->Send_CS_MONSTER_UPDATE_REQ();
			return true;
		}
	}
	return false;

	// Method 2: Not using blueprint asset
	// UWorld* world = GetOuter()->GetWorld();
	// if (!world) return nullptr;
	//
	// const FTransform spawn_transform(spawn_rotation, spawn_location);
	// AAZMonster_Client* monster = world->SpawnActorDeferred<AAZMonster_Client>(AAZMonster_Client::StaticClass(), spawn_transform);
	// monster->Init(monster_id, rank);
	// monster->FinishSpawning(spawn_transform);
	// bool result = AddObject(object_serial, monster);
	// if (result) monster->packet_handler_component_->Send_CS_MONSTER_UPDATE_REQ();
}

AAZCharacter* UAZObjectMgr_Client::GetCharacter(int32 object_serial)
{
	if (const auto object = object_map_.Find(object_serial))
	{
		return object->Get();
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Client][#%d] Object not found!"), object_serial);
		return nullptr;
	}
}

AAZMonster_Client* UAZObjectMgr_Client::GetMonster(int32 object_serial)
{
	if (const auto monster = GetCharacter(object_serial))
	{
		return static_cast<AAZMonster_Client*>(monster);
	}
	else
	{
		return nullptr;
	}
}

AAZPlayer* UAZObjectMgr_Client::GetPlayer(int32 object_serial)
{
	if (const auto player = GetCharacter(object_serial))
	{
		return static_cast<AAZPlayer*>(player);
	}
	else
	{
		return nullptr;
	}
}

void UAZObjectMgr_Client::SpawnAllMonsters()
{
	const auto gamemode = Cast<AAZGameMode_InGame>(GetOuter());
	if (!gamemode)
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UAZObjectMgr_Client] Incorrect gamemode!"));
		return;
	}
	
	SC_MONSTER_SPAWN_CMD packet;
	while (spawn_packets_.Dequeue(packet))
	{
		bool is_spawned = gamemode->object_mgr_->SpawnMonster(packet.object_serial, packet.monster_id, packet.rank, packet.location, packet.rotation);
		if (is_spawned)
		{
			UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_SPAWN_CMD] ID: %d, Location: %s, Rotation %s"),
			packet.object_serial, packet.monster_id, *packet.location.ToString(), *packet.rotation.ToString());
		}
		else
		{
			UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][Receive_SC_MONSTER_SPAWN_CMD] ID: %d, Failed to spawn!"));
		}
	}
}

