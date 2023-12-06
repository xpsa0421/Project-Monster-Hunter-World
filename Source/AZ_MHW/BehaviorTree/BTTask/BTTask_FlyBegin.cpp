// Copyright Team AZ. All Rights Reserved.


#include "BehaviorTree/BTTask/BTTask_FlyBegin.h"

#include "AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"

EBTNodeResult::Type UBTTask_FlyBegin::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto monster = Cast<AAZMonster>(owner_comp.GetAIOwner()->GetPawn());
	EBTNodeResult::Type result = EBTNodeResult::Failed;
	anim_timer_handle_.Invalidate();
	owner_comp_ = &owner_comp;
	
	if (monster)
	{
		USkeletalMeshComponent* mesh = monster->GetMesh();
		if (mesh)
		{
			prev_anim_mode_ = mesh->GetAnimationMode();
			cached_skm_ = mesh;

			monster->BeginFly();
			const float finish_delay = monster->anim_instance_->fly_start_anim_->GetPlayLength();

			FTimerDelegate anim_timer_delegate = FTimerDelegate::CreateUObject(this, &UBTTask_FlyBegin::OnAnimationTimerDone);
			owner_comp.GetAIOwner()->GetWorld()->GetTimerManager().SetTimer(anim_timer_handle_, anim_timer_delegate, finish_delay, false);

			result = EBTNodeResult::InProgress;
		}
	}

	return result;
}

EBTNodeResult::Type UBTTask_FlyBegin::AbortTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AAIController* const controller = owner_comp.GetAIOwner();
	if (controller && anim_timer_handle_.IsValid())
	{
		controller->GetWorld()->GetTimerManager().ClearTimer(anim_timer_handle_);
	}
	anim_timer_handle_.Invalidate();
	CleanUp(owner_comp);
	
	return EBTNodeResult::Aborted;
}

void UBTTask_FlyBegin::OnAnimationTimerDone()
{
	if (owner_comp_)
	{
		CleanUp(*owner_comp_);
		FinishLatentTask(*owner_comp_, EBTNodeResult::Succeeded);
	}
}

void UBTTask_FlyBegin::CleanUp(UBehaviorTreeComponent& owner_comp)
{
	if (cached_skm_ && prev_anim_mode_ == EAnimationMode::AnimationBlueprint)
	{
		//cached_skm_->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}
