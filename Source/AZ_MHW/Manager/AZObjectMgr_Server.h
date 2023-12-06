// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZObjectMgr_Server.generated.h"

class AAZCharacter;
class AAZPlayer_Origin;
class AAZMonster;
enum class EBossRank : uint8;


UCLASS()
class AZ_MHW_API UAZObjectMgr_Server : public UObject
{
	GENERATED_BODY()
	UAZObjectMgr_Server();
	virtual void BeginDestroy() override;
	
public:
	void AddObject(AAZCharacter* character);
	bool RemoveObject(int32 object_serial);
	AAZMonster* SpawnMonster(int32 monster_id, EBossRank rank, FVector spawn_location = FVector::ZeroVector, FRotator spawn_rotation = FRotator::ZeroRotator);
	
	AAZCharacter* GetObject(int32 object_serial);
	AAZMonster* GetMonster(int32 object_serial);
	AAZPlayer_Origin* GetPlayer(int32 object_serial);

	TArray<FMonsterSpawnInfo>* GetSpawnArray();
	void SpawnAllMonsters();
	
private:
	int32 object_serial_;
	//TEMP 맵 하나니까 그냥 통일 -> TODO 테이블로드
	UPROPERTY() TArray<FMonsterSpawnInfo> spawn_array_;

public:
	//TEMP 일단 한번에 스폰
	bool spawn_finished_;
	UPROPERTY() TMap<int32, TObjectPtr<AAZCharacter>> object_map_;
	UPROPERTY(BlueprintReadOnly) TArray<AAZMonster*> spawned_monsters_array_;
	UPROPERTY() TMap<int32, TObjectPtr<AAZPlayer_Origin>> player_map_;
};
