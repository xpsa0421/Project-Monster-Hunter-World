// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/BehaviorTree/BTDecorator/BTDecorator_IsInCombat.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
// -----------------------------
// CURRENTLY NOT IN USE
// -----------------------------
UBTDecorator_IsInCombat::UBTDecorator_IsInCombat()
{
	NodeName = TEXT("IsInCombat");
	blackboard_key.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInCombat, blackboard_key));
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTDecorator_IsInCombat::InitializeFromAsset(UBehaviorTree& asset)
{
	Super::InitializeFromAsset(asset);

	UBlackboardData* bb_asset = GetBlackboardAsset();
	if (ensure(bb_asset))
	{
		blackboard_key.ResolveSelectedKey(*bb_asset);
	}
}

bool UBTDecorator_IsInCombat::CalculateRawConditionValue(UBehaviorTreeComponent& owner_comp, uint8* node_memory) const
{
	UBlackboardComponent* blackboard_comp = owner_comp.GetBlackboardComponent();
	if (blackboard_comp)
	{
		bool is_in_combat = blackboard_comp->GetValueAsBool(blackboard_key.SelectedKeyName);
		return is_in_combat;
	}
	return false;
}

EBlackboardNotificationResult UBTDecorator_IsInCombat::OnBlackboardKeyValueChange(const UBlackboardComponent& blackboard, FBlackboard::FKey changed_key_id)
{
	UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)blackboard.GetBrainComponent();
	if (BehaviorComp == nullptr)
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	if (blackboard_key.GetSelectedKeyID() == changed_key_id)
	{
		bool is_in_combat = blackboard.GetValueAsBool(blackboard_key.SelectedKeyName);
		if (is_in_combat) BehaviorComp->RequestExecution(this);
	}
	return EBlackboardNotificationResult::ContinueObserving;
}

void UBTDecorator_IsInCombat::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	UBlackboardComponent* blackboard_comp = owner_comp.GetBlackboardComponent();
	if (blackboard_comp)
	{
		blackboard_comp->RegisterObserver(blackboard_key.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_IsInCombat::OnBlackboardKeyValueChange));
	}
}

void UBTDecorator_IsInCombat::OnCeaseRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	UBlackboardComponent* blackboard_comp = owner_comp.GetBlackboardComponent();
	if (blackboard_comp)
	{
		blackboard_comp->UnregisterObserversFrom(this);
	}
}

#if WITH_EDITOR

FName UBTDecorator_IsInCombat::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.CompareBlackboardEntries.Icon");
}

#endif