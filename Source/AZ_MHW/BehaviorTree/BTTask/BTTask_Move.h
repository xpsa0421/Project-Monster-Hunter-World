// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Move.generated.h"

struct FBTMoveTaskMemory
{
	// TODO: need to implement instancing when more than one monsters are referencing the same behavior tree
};

UCLASS()
class AZ_MHW_API UBTTask_Move : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Move();

	virtual void OnMessage(UBehaviorTreeComponent& owner_comp, uint8* node_memory, FName message, int32 sender_id, bool is_success) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& owner_comp, uint8* node_memory, EBTNodeResult::Type task_result) override;

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	EBTNodeResult::Type MoveBegin(UBehaviorTreeComponent& owner_comp);

private:
	UPROPERTY(EditAnywhere) FVector final_dest_;
	UPROPERTY(VisibleAnywhere) int32 path_index_;
	UPROPERTY(VisibleAnywhere) TArray<FVector> path_points_;

	TObjectPtr<class AAZMonster> owner_;

private:
	FAIRequestID move_request_id_;
};
