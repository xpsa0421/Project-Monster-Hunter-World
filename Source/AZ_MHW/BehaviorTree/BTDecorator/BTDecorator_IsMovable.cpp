// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/BehaviorTree/BTDecorator/BTDecorator_IsMovable.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
// -----------------------------
// CURRENTLY NOT IN USE
// -----------------------------
UBTDecorator_IsMovable::UBTDecorator_IsMovable()
{
	NodeName = TEXT("IsMovable");
	bAllowAbortNone = false;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	blackboard_key.AddEnumFilter(
		this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsMovable, blackboard_key), StaticEnum<ECharacterState>());
}

void UBTDecorator_IsMovable::InitializeFromAsset(UBehaviorTree& asset)
{
	Super::InitializeFromAsset(asset);

	UBlackboardData* bb_asset = GetBlackboardAsset();
	if (ensure(bb_asset))
	{
		blackboard_key.ResolveSelectedKey(*bb_asset);
	}
}

bool UBTDecorator_IsMovable::CalculateRawConditionValue(UBehaviorTreeComponent& owner_comp, uint8* node_memory) const
{
	UBlackboardComponent* blackboard_comp = owner_comp.GetBlackboardComponent();
	if (blackboard_comp)
	{
		ECharacterState character_state = ECharacterState(blackboard_comp->GetValueAsEnum(blackboard_key.SelectedKeyName));
		return character_state == ECharacterState::Locomotion;
	}
	return false;
}

EBlackboardNotificationResult UBTDecorator_IsMovable::OnBlackboardKeyValueChange(const UBlackboardComponent& blackboard, FBlackboard::FKey changed_key_id)
{
	UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)blackboard.GetBrainComponent();
	if (BehaviorComp == nullptr)
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	if (blackboard_key.GetSelectedKeyID() == changed_key_id)
	{
		ECharacterState character_state = ECharacterState(blackboard.GetValueAsEnum(blackboard_key.SelectedKeyName));
		if (character_state == ECharacterState::Locomotion) BehaviorComp->RequestExecution(this);
	}
	return EBlackboardNotificationResult::ContinueObserving;
}

void UBTDecorator_IsMovable::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	UBlackboardComponent* blackboard_comp = owner_comp.GetBlackboardComponent();
	if (blackboard_comp)
	{
		blackboard_comp->RegisterObserver(blackboard_key.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_IsMovable::OnBlackboardKeyValueChange));
	}
}

void UBTDecorator_IsMovable::OnCeaseRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	UBlackboardComponent* blackboard_comp = owner_comp.GetBlackboardComponent();
	if (blackboard_comp)
	{
		blackboard_comp->UnregisterObserversFrom(this);
	}
}

#if WITH_EDITOR

FName UBTDecorator_IsMovable::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.CompareBlackboardEntries.Icon");
}

#endif