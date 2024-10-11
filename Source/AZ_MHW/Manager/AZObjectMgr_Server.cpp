// Copyright Team AZ. All Rights Reserved.


#include "AZObjectMgr_Server.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Character/Player/AZPlayer_Origin.h"

UAZObjectMgr_Server::UAZObjectMgr_Server()
{
	object_serial_ = -1;
	
	// TEMP
	//spawn_array_.Add(FMonsterSpawnInfo(0, EBossRank::Low, FVector(2010,-110,200), FRotator(0,180,0)));
	spawn_array_.Add(FMonsterSpawnInfo(0, EBossRank::Low, FVector(70000,41558.0,6330.0), FRotator(0,0,0)));
}

void UAZObjectMgr_Server::BeginDestroy()
{
	UObject::BeginDestroy();
	object_map_.Empty();
}

void UAZObjectMgr_Server::AddObject(AAZCharacter* character)
{
	object_map_.Add(++object_serial_, character);
	character->object_serial_ = object_serial_;
	if (AAZMonster* monster = Cast<AAZMonster>(character))
	{
		spawned_monsters_array_.Add(monster);
	}
	else if (AAZPlayer_Origin* player = Cast<AAZPlayer_Origin>(character))
	{
		player_map_.Add(object_serial_, player);
	}
	UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Server] %s added. Serial No: %d"), *character->GetName(), object_serial_);
}

bool UAZObjectMgr_Server::RemoveObject(int32 object_serial)
{
	if (object_map_.Remove(object_serial))
	{
		UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Server] Serial No: %d is removed"), object_serial);
		return true;
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Server] Serial No: %d does not exist!"), object_serial);
		return false;
	}
}

AAZMonster* UAZObjectMgr_Server::SpawnMonster(int32 monster_id, EBossRank rank, FVector spawn_location, FRotator spawn_rotation)
{
	UWorld* world = GetOuter()->GetWorld();
	if (!world) return nullptr;

	const FTransform spawn_transform(spawn_rotation, spawn_location);
	AAZMonster* monster = world->SpawnActorDeferred<AAZMonster>(AAZMonster::StaticClass(),
		spawn_transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	monster->Init(monster_id, rank);
	monster->FinishSpawning(spawn_transform);
	AddObject(monster);
		
	return monster;
}

AAZCharacter* UAZObjectMgr_Server::GetObject(int32 object_serial)
{
	if (const auto object = object_map_.Find(object_serial))
	{
		return object->Get();
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Server][#%d] Object not found!"), object_serial);
		return nullptr;
	}
}

AAZMonster* UAZObjectMgr_Server::GetMonster(int32 object_serial)
{
	if (const auto monster = GetObject(object_serial))
	{
		return static_cast<AAZMonster*>(monster);
	}
	else
	{
		return nullptr;
	}
}

AAZPlayer_Origin* UAZObjectMgr_Server::GetPlayer(int32 object_serial)
{
	if (const auto player = GetObject(object_serial))
	{
		return static_cast<AAZPlayer_Origin*>(player);
	}
	else
	{
		return nullptr;
	}
}

// 따로 빼야됨
TArray<FMonsterSpawnInfo>* UAZObjectMgr_Server::GetSpawnArray()
{
	return &spawn_array_;
}

void UAZObjectMgr_Server::SpawnAllMonsters()
{
	if (spawn_finished_) return;
	for (auto info : spawn_array_)
	{
		SpawnMonster(info.monster_id, info.rank, info.location, info.rotation);
	}
	spawn_finished_ = true;	
}
