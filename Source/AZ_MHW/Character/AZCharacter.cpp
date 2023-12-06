// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/Character/AZCharacter.h"

#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AAZCharacter::AAZCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	SetGenericTeamId(static_cast<uint8>(EObjectType::None));
	object_serial_ = -1;
}

void AAZCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AAZCharacter::SetGenericTeamId(const FGenericTeamId& team_id)
{
	team_id_ = team_id;
}

FGenericTeamId AAZCharacter::GetGenericTeamId() const
{
	return team_id_;
}

void AAZCharacter::AnimNotify_EndOfAction()
{
	// empty in base class
}

void AAZCharacter::AnimNotify_JumpToAnimation(FString next_animation_name, FString next_montage_section_name)
{
	// empty in base class
}

void AAZCharacter::AnimNotify_SetMovementMode(EMovementMode movement_mode)
{
	// empty in base class
}

void AAZCharacter::AnimNotify_DoSphereOverlap(FName socket_name, float radius)
{
	// empty in base class
}

float AAZCharacter::GetRelativeAngleToLocation(const FVector& target_location) const
{
	const FRotator rotator = UKismetMathLibrary::FindRelativeLookAtRotation(GetActorTransform(), target_location);
	if (rotator.IsNearlyZero())
		return 0.0f;
	else
		return rotator.Yaw;
}

float AAZCharacter::GetDistance2DToLocation(const FVector& target_location) const
{
	return FVector2D::Distance(FVector2D(GetActorLocation()), FVector2D(target_location));
}

float AAZCharacter::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result,
	FAttackInfo attack_info)
{
	return IAZDamageAgentInterface::ApplyDamage_Implementation(damaged_actor, hit_result, attack_info);
}

float AAZCharacter::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	return IAZDamageAgentInterface::ProcessDamage(damage_instigator, hit_result, attack_info);
}
