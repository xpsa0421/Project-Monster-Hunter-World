// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/BehaviorTree/BTService/BTService_FindTarget.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/CharacterComponent/AZMonsterAggroComponent.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>

#include "Character/Player/AZPlayer_Origin.h"

UBTService_FindTarget::UBTService_FindTarget(const FObjectInitializer& object_initializer) : Super(object_initializer)
{
	NodeName = "FindTarget";
	Interval = update_interval_;
	blackboard_key.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindTarget, blackboard_key), AAZCharacter::StaticClass());
}

void UBTService_FindTarget::InitializeFromAsset(UBehaviorTree& asset)
{
	Super::InitializeFromAsset(asset);

	UBlackboardData* bb_asset = GetBlackboardAsset();
	if (ensure(bb_asset))
	{
		blackboard_key.ResolveSelectedKey(*bb_asset);
	}
}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& owner_comp, uint8* node_memory, float delta_seconds)
{
	Super::TickNode(owner_comp, node_memory, delta_seconds);

	AAZMonster* owner = Cast<AAZMonster>(owner_comp.GetAIOwner()->GetPawn());
	if (!owner) return;

	owner->aggro_component_->UpdateBestTarget();
	auto target_character = owner->aggro_component_->GetTargetRef();	
	owner_comp.GetBlackboardComponent()->SetValueAsObject(blackboard_key.SelectedKeyName, target_character);
}
