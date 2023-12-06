// Copyright Team AZ. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonster_Client.generated.h"

class AAZPlayer_Playable;
// Forward declaration of classes
class UAZAnimInstance_Monster;
class UAZMonsterMeshComponent_Client;
class UAZMonsterPacketHandlerComponent_Client;

UCLASS(Blueprintable)
class AZ_MHW_API AAZMonster_Client : public AAZCharacter
{
	GENERATED_BODY()

public:
	AAZMonster_Client();
	
	// Overrides
	virtual void BeginPlay() override;

	// Setters
	void Init(int32 monster_id, EBossRank rank);
	void SetMeshAndColliders();
	void SetActionStateInfo(const FMonsterActionStateInfo action_state_info);
	void AnimNotify_SetMovementMode(EMovementMode movement_mode);
	UFUNCTION() void ProcessDeath();
	
	// Getters
	bool IsABoss() const;
	
	// Delegates
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWoundedSignature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWoundHealedSignature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBrokenSigature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSeveredSigature, EMonsterBodyPart)

	// Dynamic delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnragedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitSignature, AAZPlayer_Playable*, player, FHitResultInfo, hit_result);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnrageEndedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterCombatSignature);

public:	
	// Components
	TObjectPtr<UAZMonsterMeshComponent_Client> mesh_component_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") TObjectPtr<UAZAnimInstance_Monster> anim_instance_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") TObjectPtr<UAZMonsterPacketHandlerComponent_Client> packet_handler_component_;
	
protected:
	// Properties
	UPROPERTY(VisibleAnywhere, Category = "AZ") int32 monster_id_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") EBossRank rank_;

public:
	UPROPERTY(VisibleAnywhere, Category = "AZ") FName name_;
	
	// States
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FMonsterActionStateInfo action_state_info_;
	UPROPERTY(VisibleAnywhere) bool is_dead_;

	//TEMP
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool temp_roar_affected_;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) void EndRoar();
	
public:
	// Delegates
	FOnWoundedSignature OnBodyPartWounded;
	FOnWoundHealedSignature OnBodyPartWoundHealed;
	FOnBrokenSigature OnBodyPartBroken;
	FOnSeveredSigature OnBodyPartSevered;

	// Dynamic delegates
	UPROPERTY(BlueprintAssignable) FOnHitSignature OnHit;
	UPROPERTY(BlueprintAssignable) FOnEnragedSignature OnEnraged;
	UPROPERTY(BlueprintAssignable) FOnEnrageEndedSignature OnEnrageEnded;
	UPROPERTY(BlueprintAssignable) FOnDeathSignature OnDeath;
	UPROPERTY(BlueprintAssignable) FOnEnterCombatSignature OnEnterCombat;
};
