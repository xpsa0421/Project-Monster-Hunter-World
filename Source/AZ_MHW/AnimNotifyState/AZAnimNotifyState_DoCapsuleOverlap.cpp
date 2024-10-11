// Copyright Team AZ. All Rights Reserved.

#include "AZAnimNotifyState_DoCapsuleOverlap.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "Components/CapsuleComponent.h"

UAZAnimNotifyState_DoCapsuleOverlap::UAZAnimNotifyState_DoCapsuleOverlap()
{
	check_interval_ = 0.5f;
}

void UAZAnimNotifyState_DoCapsuleOverlap::NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
                                                 float total_duration, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyBegin(mesh_comp, animation, total_duration, event_reference);
	if (mesh_comp == nullptr || mesh_comp->GetOwner() == nullptr)
		return;

	if (mesh_comp->GetOwner()->IsA(AAZMonster::StaticClass()))
	{
		AAZMonster* monster = Cast<AAZMonster>(mesh_comp->GetOwner());
		if (monster)
		{
			UE_LOG(AZMonster, Warning, TEXT("Begin CapsuleOverlap: interval %f, radius %f, half_height %f"), check_interval_,  sphere_radius_, sphere_half_height_);
			monster->AnimNotifyState_DoCapsuleOverlap_Begin();
		}
	}
}

void UAZAnimNotifyState_DoCapsuleOverlap::NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	float total_duration, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyTick(mesh_comp, animation, total_duration, event_reference);
	if (mesh_comp == nullptr || mesh_comp->GetOwner() == nullptr)
		return;

	if (mesh_comp->GetOwner()->IsA(AAZMonster::StaticClass()))
	{
		AAZMonster* monster = Cast<AAZMonster>(mesh_comp->GetOwner());
		if (monster)
		{
			monster->AnimNotifyState_DoCapsuleOverlap_Tick(socket_name_, sphere_radius_, sphere_half_height_, check_interval_);
		}
	}
}
