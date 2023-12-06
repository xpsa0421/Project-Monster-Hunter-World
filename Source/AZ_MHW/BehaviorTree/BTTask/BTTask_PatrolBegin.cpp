// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/BehaviorTree/BTTask/BTTask_PatrolBegin.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include <AZ_MHW/Character/Monster/AZMonster.h>

#include "AZ_MHW/Util/AZUtility.h"

UBTTask_PatrolBegin::UBTTask_PatrolBegin()
{
	NodeName = "Begin Patrol";
	key_destination_.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PatrolBegin, key_destination_));
	key_patrol_delay_.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PatrolBegin, key_patrol_delay_));
}

void UBTTask_PatrolBegin::InitializeFromAsset(UBehaviorTree& asset)
{
	Super::InitializeFromAsset(asset);
	UBlackboardData* bb_asset = GetBlackboardAsset();
	if (ensure(bb_asset))
	{
		key_destination_.ResolveSelectedKey(*bb_asset);
		key_patrol_delay_.ResolveSelectedKey(*bb_asset);
	}
}

EBTNodeResult::Type UBTTask_PatrolBegin::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto ai_controller = Cast<AAZAIController>(owner_comp.GetAIOwner());
	if (!ai_controller)
		return EBTNodeResult::Failed;

	auto ai_pawn = ai_controller->GetPawn();
	if (!ai_pawn)
		return EBTNodeResult::Failed;

	double patrol_range = ai_controller->patrol_range_;
	if (!patrol_range)
	{
		UE_LOG(AZMonster, Warning, TEXT("[BTTask_PatrolBegin] Monster's patrol range is not set."));
		return EBTNodeResult::Failed;	
	}

	UWorld* world = ai_pawn->GetWorld();
	if (!world)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* nav_system = UNavigationSystemV1::GetCurrent(world);
	if (!nav_system)
	{
		UE_LOG(AZMonster, Warning, TEXT("[BTTask_PatrolBegin] Navigation volume is not found."));
		return EBTNodeResult::Failed;	
	}

	//TODO: 주변에 플레이어 없으면 patrol 하지 않음 
	//TODO: 스폰지점보다 멀면 스폰지점으로 이동

	FVector curr_position = ai_pawn->GetActorLocation();
	FVector dest_position = FVector::ZeroVector;
	FNavLocation found_position = FNavLocation();

	if (nav_system->GetRandomPointInNavigableRadius(curr_position, patrol_range, found_position))
	{
		dest_position = found_position.Location;
	}
	else
	{
		UE_LOG(AZMonster, Warning, TEXT("[BTTask_PatrolBegin] No navigable point in radius."));
		return EBTNodeResult::Failed;	
	}

	owner_comp.GetBlackboardComponent()->SetValueAsVector(key_destination_.SelectedKeyName, dest_position);
	owner_comp.GetBlackboardComponent()->SetValueAsFloat(key_patrol_delay_.SelectedKeyName, ai_controller->patrol_delay_);

	return EBTNodeResult::Succeeded;
}
