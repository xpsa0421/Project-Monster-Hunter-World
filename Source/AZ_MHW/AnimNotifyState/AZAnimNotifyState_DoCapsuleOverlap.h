// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZAnimNotifyState_DoCapsuleOverlap.generated.h"

UCLASS()
class AZ_MHW_API UAZAnimNotifyState_DoCapsuleOverlap : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAZAnimNotifyState_DoCapsuleOverlap();

	virtual void NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration, const FAnimNotifyEventReference& event_reference) override;
	virtual void NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere) FName socket_name_;
	UPROPERTY(EditAnywhere) float sphere_radius_;
	UPROPERTY(EditAnywhere) float sphere_half_height_;
	UPROPERTY(EditAnywhere) float check_interval_;
};
