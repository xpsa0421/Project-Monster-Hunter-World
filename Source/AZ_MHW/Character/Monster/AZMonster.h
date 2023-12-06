// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonster.generated.h"

// Forward declaration of classes
class UAIPerceptionStimuliSourceComponent;
class UAZAnimInstance_Monster;
class UAZMonsterAggroComponent;
class UAZMonsterHealthComponent;
class UAZMonsterMeshComponent;
class UAZMonsterPacketHandlerComponent;
class AAZAIController;
class UAnimInstance;
class UAnimBlueprint;

// Forward declaration of enum classes
enum class EMoveState : uint8;
enum class EMonsterActionMode : uint8;
enum class EEffectDurationType : uint8;
enum class EBossRank : uint8;
enum class EMonsterBehaviorType : uint8;


UCLASS(Blueprintable)
class AZ_MHW_API AAZMonster : public AAZCharacter
{
	GENERATED_BODY()
	
public:
	AAZMonster();
	
	// Overrides
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& out_location, FRotator& out_rotation) const override;

	// Property Initialisers
	void Init(int32 monster_id, EBossRank rank);
	void SetMeshAndColliders();
	void SetUpDefaultProperties();
	void SetMonsterInfo();
	void SetBossInfo();
	void SetActionInfo();
	void InitializeRuntimeValues();
	
	// State Setters
	void EnterCombat(AActor* combat_instigator, bool is_triggered_by_sight);
	void SetActionMode(EMonsterActionMode action_mode);
	UFUNCTION(BlueprintCallable) void SetMoveState(EMoveState move_state);
	void SetTargetAngle(float angle);
	void SetActionState(int32 action_id);
	void ResetTargetAngle();
	void SetEnraged(bool is_enraged);
	void BeginFly();
	void EndFly();
	UFUNCTION() void ProcessDeath();

	// Property Getters
	int32 GetMonsterID() const;
	EBossRank GetBossRank() const;
	EMonsterBehaviorType GetBehaviorType() const;
	AAZAIController* GetController();

	// State Checkers
	bool IsInCombat() const;
	bool IsFlying() const;
	bool IsMoving() const;
	bool IsEnraged() const;
	
	// AnimNotifyHandlers
	// TODO as a component?
	virtual void AnimNotify_EndOfAction() override;
	virtual void AnimNotify_JumpToAnimation(FString next_animation_name, FString next_montage_section_name) override;
	virtual void AnimNotify_SetMovementMode(EMovementMode movement_mode) override;
	virtual void AnimNotify_DoSphereOverlap(FName socket_name, float radius) override;
	void AnimNotifyState_DoCapsuleOverlap_Begin();
	void AnimNotifyState_DoCapsuleOverlap_Tick(FName socket_name, float radius, float half_height, float check_interval);
	
	// Validity Checkers
	bool IsABoss() const;
	bool IsAValidMonster() const;

protected:
	// Damage
	UFUNCTION(BlueprintCallable) void DoDamage(AActor* damaged_actor, const FHitResult hit_result);
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info) override;

	// Delegates
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWoundedSignature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWoundHealedSignature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBrokenSigature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSeveredSigature, EMonsterBodyPart)

	// Dynamic delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitSignature, AAZPlayer_Origin*, attacker, FHitResultInfo, hit_result);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnragedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnrageEndedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterCombatSignature);
	
public:
	virtual float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info) override;
	
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
	
	// BlueprintReadWrite FOR DEBUG ONLY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Monster | States") FMonsterActionStateInfo action_state_info_;
	
	// Components
	UPROPERTY() TObjectPtr<UCapsuleComponent> body_capsule_component_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAZMonsterAggroComponent> aggro_component_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAZMonsterHealthComponent> health_component_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAZMonsterMeshComponent> mesh_component_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAZMonsterPacketHandlerComponent> packet_handler_component_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Animation") TObjectPtr<UAZAnimInstance_Monster> anim_instance_;
	
	// AIController Properties: Defined from table
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") FMonsterSightConfigs sight_configs_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") int32 patrol_range_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") float patrol_delay_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") int32 percept_radius_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") FName name_;
	UPROPERTY(EditAnywhere, Category = "AZ | Monster | AIController") float acceptance_radius_;

	// Animation: Defined from table
	UPROPERTY(VisibleAnywhere, Category = "AZ | Animation")	TMap<int32, FMonsterCombatActionInfo> combat_action_map_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Animation")	TMap<int32, FMonsterNonCombatActionInfo> noncombat_action_map_;
	
	// Monster base information set from blueprint
	UPROPERTY(EditAnywhere, Category = "AZ | Monster | Properties") int32 monster_id_;
	UPROPERTY(EditAnywhere, Category = "AZ | Boss") EBossRank rank_;
	
	// Properties: Defined from table
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | Properties") EMonsterBehaviorType behavior_type_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Boss") int32 boss_id_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Boss") bool has_combat_transition_anim_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Boss") FBossRageStats rage_stats_; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Others")
	TArray<TEnumAsByte<EObjectTypeQuery>> hit_object_types_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Monster | States") bool is_flying_;
	
protected:
	// Properties: Defined runtime
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | States") bool is_enraged_;
	bool is_dead_;
	
	// Other properties
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster") int32 active_action_id_;
	UPROPERTY() TArray<AActor*> overlapped_actors_;
	UPROPERTY() float overlap_check_timer_;
};
