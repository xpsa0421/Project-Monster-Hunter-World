// Fill out your copyright notice in the Description page of Project Settings.


#include "AZAnimNotifyState.h"

#include "Util/AZUtility.h"

UAZAnimNotifyState::UAZAnimNotifyState()
{
	
}

void UAZAnimNotifyState::NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	float total_duration, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyBegin(mesh_comp, animation, total_duration, event_reference);
	UE_LOG(AZ_TEST,Error,TEXT("NotifyState Begin"));
}

void UAZAnimNotifyState::NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	float frame_delta_time, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyTick(mesh_comp, animation, frame_delta_time, event_reference);
	UE_LOG(AZ_TEST,Error,TEXT("NotifyState Tick"));
}

void UAZAnimNotifyState::NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyEnd(mesh_comp, animation, event_reference);
	UE_LOG(AZ_TEST,Error,TEXT("NotifyState End"));
}
