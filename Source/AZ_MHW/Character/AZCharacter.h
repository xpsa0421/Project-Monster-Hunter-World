// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GenericTeamAgentInterface.h>
#include <GameFramework/Character.h>
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/Interface/AZDamageAgentInterface.h"
#include "AZCharacter.generated.h"

/**
* PlayerController나 AIController가 빙의가능(조종)하고, 여러가지 향상된 폰
* Actor-> Pawn(빙의가능)-> Character(기본구현 및 움직임 개선)
* AZ_ Player, NPC, Enemy?Monster? Base Clas
*/
UCLASS(Abstract)
class AZ_MHW_API AAZCharacter : public ACharacter,
	public IGenericTeamAgentInterface,
	public IAZDamageAgentInterface
{
	GENERATED_BODY()

public:
	AAZCharacter();

protected:
	// AI Module
	FGenericTeamId team_id_;
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(VisibleAnywhere) int32 object_serial_;
	
	// GenericTeamAgent Interface
	virtual void SetGenericTeamId(const FGenericTeamId& team_id) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	// AnimNotify Handlers
	virtual void AnimNotify_EndOfAction();
	virtual void AnimNotify_JumpToAnimation(FString next_animation_name, FString next_montage_section_name);
	virtual void AnimNotify_SetMovementMode(EMovementMode movement_mode);
	virtual void AnimNotify_DoSphereOverlap(FName socket_name, float radius);
	
	// Others
	float GetRelativeAngleToLocation(const FVector& target_location) const;
	float GetDistance2DToLocation(const FVector& target_location) const;

	// Damage Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) float ApplyDamage(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info);
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info) override;
	virtual float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info) override;
};
