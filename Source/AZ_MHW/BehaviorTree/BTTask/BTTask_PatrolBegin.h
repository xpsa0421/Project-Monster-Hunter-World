// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolBegin.generated.h"

UCLASS(Blueprintable)
class AZ_MHW_API UBTTask_PatrolBegin : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PatrolBegin();
	virtual void InitializeFromAsset(UBehaviorTree& asset) override;

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AZ") struct FBlackboardKeySelector key_destination_;
	UPROPERTY(EditAnywhere, Category = "AZ") struct FBlackboardKeySelector key_patrol_delay_;
};
