// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SelectRandomAction.generated.h"

class AAZMonster;

UCLASS()
class AZ_MHW_API UBTTask_SelectRandomAction : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SelectRandomAction(const FObjectInitializer& object_initializer);
	virtual void InitializeFromAsset(UBehaviorTree& asset) override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	EBTNodeResult::Type SelectNonCombatAction(AAZMonster* owner, UBlackboardComponent* blackboard);
	EBTNodeResult::Type SelectTransitionAction(AAZMonster* owner, UBlackboardComponent* blackboard);
	EBTNodeResult::Type SelectCombatAction(AAZMonster* owner, UBlackboardComponent* blackboard);

protected:
	/* only used in non combat mode*/
	UPROPERTY(EditAnywhere, Category = "AZ") bool is_player_in_range_;
	
	UPROPERTY(EditAnywhere, Category = "AZ") struct FBlackboardKeySelector key_is_triggered_by_sight_;
	UPROPERTY(EditAnywhere, Category = "AZ") struct FBlackboardKeySelector key_action_mode_;
	UPROPERTY(EditAnywhere, Category = "AZ") struct FBlackboardKeySelector key_target_character_;
	UPROPERTY(EditAnywhere, Category = "AZ") struct FBlackboardKeySelector key_action_delay_;
};
