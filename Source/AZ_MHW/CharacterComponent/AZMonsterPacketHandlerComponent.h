// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonsterPacketHandlerComponent.generated.h"

class AAZMonster;
class UAZGameInstance;
class AAZPlayer_Origin;
enum class EMonsterBodyPart : uint8;
enum class EMonsterBodyPartChangeType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AZ_MHW_API UAZMonsterPacketHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAZMonsterPacketHandlerComponent();
	void Init();
	virtual void BeginPlay() override;

	// Server to Client
	void Send_SC_MONSTER_SPAWN_CMD(UINT32 newly_joined_client_idx);
	void Send_SC_MONSTER_TRANSFORM_CMD(bool is_forced = true, UINT32 newly_joined_client_idx = 0);
	void Send_SC_MONSTER_BODY_STATE_CMD(UINT32 newly_joined_client_idx);
	UFUNCTION() void Send_SC_MONSTER_ENTER_COMBAT_CMD();
	void Send_SC_MONSTER_ACTION_START_CMD(float start_position = 0.0f, UINT32 newly_joined_client_idx = 0);
	void Send_SC_MONSTER_ACTION_END_CMD();
	void Send_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type);
	UFUNCTION() void Send_SC_MONSTER_HIT_CMD(AAZPlayer_Origin* attacker, FHitResultInfo hit_info);
	UFUNCTION() void Send_SC_MONSTER_ENRAGE_BEGIN_CMD();
	UFUNCTION() void Send_SC_MONSTER_ENRAGE_END_CMD();
	UFUNCTION() void Send_SC_MONSTER_DIE_CMD();

	// Client to Server
	void Receive_CS_MONSTER_UPDATE_REQ(UINT32 client_index);

	// Delegate binds
	UFUNCTION() void OnBodyPartWounded(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartBroken(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartSevered(EMonsterBodyPart body_part);
	
private:
	TWeakObjectPtr<AAZMonster> owner_;
	TObjectPtr<UAZGameInstance> game_instance_;
};
