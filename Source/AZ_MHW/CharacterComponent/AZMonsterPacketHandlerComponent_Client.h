// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonSource/AZStruct.h"
#include "Components/ActorComponent.h"
#include "AZMonsterPacketHandlerComponent_Client.generated.h"

class AAZMonster_Client;
class UAZGameInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AZ_MHW_API UAZMonsterPacketHandlerComponent_Client : public UActorComponent
{
	GENERATED_BODY()

public:
	UAZMonsterPacketHandlerComponent_Client();
	void Init();

public:
	// Client->Server
	void Send_CS_MONSTER_UPDATE_REQ();
	
	// Handle server->client packets
	void Receive_SC_MONSTER_TRANSFORM_CMD(FVector location, FRotator rotation, bool is_forced);
	void Receive_SC_MONSTER_BODY_STATE_CMD(const FBossBodyPartDebuffState head_state, const FBossBodyPartDebuffState back_state,
	                                       const FBossBodyPartDebuffState left_wing_state, const FBossBodyPartDebuffState right_wing_state,
	                                       const FBossBodyPartDebuffState tail_state);
	void Receive_SC_MONSTER_ENTER_COMBAT_CMD();
	void Receive_SC_MONSTER_ACTION_START_CMD(const FMonsterActionStateInfo action_info, float start_positon);
	void Receive_SC_MONSTER_ACTION_END_CMD(FVector location, FRotator rotation, EMonsterActionMode action_mode);
	void Receive_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type);
	void Receive_SC_MONSTER_HIT_CMD(const FHitResultInfo hit_info);
	void Receive_SC_MONSTER_ENRAGE_BEGIN_CMD();
	void Receive_SC_MONSTER_ENRAGE_END_CMD();
	void Receive_SC_MONSTER_DIE_CMD();

private:
	TObjectPtr<AAZMonster_Client> owner_;
	TObjectPtr<UAZGameInstance> game_instance_;
};
