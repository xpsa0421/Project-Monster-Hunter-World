// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZObjectMgr_Client.generated.h"

class AAZCharacter;
class AAZPlayer;
class AAZMonster_Client;
enum class EBossRank : uint8;


UCLASS()
class AZ_MHW_API UAZObjectMgr_Client : public UObject
{
	GENERATED_BODY()
	UAZObjectMgr_Client();
	virtual void BeginDestroy() override;
	
public:
	bool AddObject(int32 object_serial, AAZCharacter* character);
	bool RemoveObject(int32 object_serial);
	bool SpawnMonster(int32 object_serial, int32 monster_id, EBossRank rank, FVector spawn_location, FRotator spawn_rotation);
	
	AAZCharacter* GetCharacter(int32 object_serial);
	AAZMonster_Client* GetMonster(int32 object_serial);
	AAZPlayer* GetPlayer(int32 object_serial);

	// packet related
	void SpawnAllMonsters();
	
private:
	UPROPERTY() TMap<int32, TObjectPtr<AAZCharacter>> object_map_;

public:
	TQueue<SC_MONSTER_SPAWN_CMD> spawn_packets_;
};
