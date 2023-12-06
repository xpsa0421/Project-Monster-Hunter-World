// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyBegin.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UBTTask_FlyBegin : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	void CleanUp(UBehaviorTreeComponent& owner_comp);
	
public:	
	void OnAnimationTimerDone();
	
protected:
	TObjectPtr<USkeletalMeshComponent> cached_skm_;
	TObjectPtr<UBehaviorTreeComponent> owner_comp_;
	EAnimationMode::Type prev_anim_mode_;
	FTimerHandle anim_timer_handle_;
};
