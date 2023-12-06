#include "AZ_MHW/BehaviorTree//BTTask/BTTask_SelectRandomAction.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>

#include "AIController.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"


UBTTask_SelectRandomAction::UBTTask_SelectRandomAction(const FObjectInitializer& object_initializer)
{
	NodeName = "Select Action";
	key_is_triggered_by_sight_.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAction, key_is_triggered_by_sight_));
	key_action_mode_.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAction, key_action_mode_), StaticEnum<EMonsterActionMode>());
	key_target_character_.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAction, key_target_character_), AActor::StaticClass());
	key_action_delay_.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAction, key_action_delay_));
}

void UBTTask_SelectRandomAction::InitializeFromAsset(UBehaviorTree& asset)
{
	Super::InitializeFromAsset(asset);
	UBlackboardData* bb_asset = GetBlackboardAsset();
	if (ensure(bb_asset))
	{
		key_is_triggered_by_sight_.ResolveSelectedKey(*bb_asset);
		key_action_mode_.ResolveSelectedKey(*bb_asset);
		key_target_character_.ResolveSelectedKey(*bb_asset);
		key_action_delay_.ResolveSelectedKey(*bb_asset);
	}
}

EBTNodeResult::Type UBTTask_SelectRandomAction::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AAZMonster* owner = Cast<AAZMonster>(owner_comp.GetAIOwner()->GetPawn());
	UBlackboardComponent* bb_comp = owner_comp.GetBlackboardComponent();
	
	EMonsterActionMode action_mode = static_cast<EMonsterActionMode>(owner_comp.GetBlackboardComponent()->GetValueAsEnum(key_action_mode_.SelectedKeyName));
	switch (action_mode)
	{
	case EMonsterActionMode::Normal:
		{
			return SelectNonCombatAction(owner, bb_comp);
		}
	case EMonsterActionMode::Transition:
		{
			return SelectTransitionAction(owner, bb_comp);
		}
	case EMonsterActionMode::Combat:
		{
			return SelectCombatAction(owner, bb_comp);
		}
	default:
		return EBTNodeResult::Failed;
	}
}

EBTNodeResult::Type UBTTask_SelectRandomAction::SelectNonCombatAction(AAZMonster* owner, UBlackboardComponent* blackboard)
{
	TArray<int32> available_action_ids;
	for (auto action_info : owner->noncombat_action_map_)
	{
		const EMonsterActionConditionType conditions = action_info.Value.conditions;
		if ((conditions & EMonsterActionConditionType::Flying) == EMonsterActionConditionType::Flying)
		{
			// Unacceptable if the owner should be flying but is not
			if (!owner->IsFlying()) continue;
		}
		else
		{
			// Unacceptable if the owner should not be flying but is
			if (owner->IsFlying()) continue;
		}
		if ((conditions & EMonsterActionConditionType::InRange) == EMonsterActionConditionType::InRange)
		{
			// Unacceptable if a player should be in the perceivable distance but is not
			if (!is_player_in_range_) continue;
		}
		if ((conditions & EMonsterActionConditionType::Health) == EMonsterActionConditionType::Health)
		{
			// Unacceptable if the owner health is not within the range
			if (owner->health_component_->GetHealthRatio() < action_info.Value.condition_min_health_ratio) continue;
			if (owner->health_component_->GetHealthRatio() > action_info.Value.condition_max_health_ratio) continue;
		}
		// acceptable if all conditions are met
		available_action_ids.Add(action_info.Key);
	}

	// If there is no available action, return fail
	if (available_action_ids.IsEmpty())
	{
		UE_LOG(AZMonster, Warning, TEXT("BT: NO ACTION"));
		return EBTNodeResult::Failed;
	}
	
	// Select random action from available actions
	const int idx = FMath::RandRange(0, available_action_ids.Num()-1);
	UE_LOG(AZMonster, Warning, TEXT("BT: Action index selected: %d"), available_action_ids[idx]);
	owner->SetActionState(available_action_ids[idx]);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_SelectRandomAction::SelectTransitionAction(AAZMonster* owner, UBlackboardComponent* blackboard)
{
	TArray<int32> available_action_ids;
	for (auto action_info : owner->combat_action_map_)
	{
		// Filter out non-transition actions
		const EMonsterActionTriggerType triggers = action_info.Value.triggers;
		if ((triggers & EMonsterActionTriggerType::Transition) != EMonsterActionTriggerType::Transition)
		{
			continue;
		}
		// If the action is only triggered by sight, check if the trigger succeeded
		if (((triggers & EMonsterActionTriggerType::Damage) != EMonsterActionTriggerType::Damage)
			&& ((triggers & EMonsterActionTriggerType::Sight) == EMonsterActionTriggerType::Sight))
		{
			if (!blackboard->GetValueAsBool(key_is_triggered_by_sight_.SelectedKeyName))
			{
				continue;
			}
		}
		// Filter out actions that fail the conditions
		// Transition action condition checks omit range and health conditions
		const EMonsterActionConditionType conditions = action_info.Value.conditions;
		if ((conditions & EMonsterActionConditionType::Flying) == EMonsterActionConditionType::Flying)
		{
			// Unacceptable if the owner should be flying but is not
			if (!owner->IsFlying()) continue;
		}
		else
		{
			// Unacceptable if the owner should not be flying but is
			if (owner->IsFlying()) continue;
		}
		// acceptable if all conditions are met
		available_action_ids.Add(action_info.Key);
	}

	// If there is no available action, return fail
	if (available_action_ids.IsEmpty())
		return EBTNodeResult::Failed;
	
	// Select random action from available actions
	const int idx = FMath::RandRange(0, available_action_ids.Num()-1);
	owner->SetActionState(available_action_ids[idx]);

	// Set action delay
	float attack_delay = owner->combat_action_map_.Find(available_action_ids[idx])->attack_delay;
	blackboard->SetValueAsFloat(key_action_delay_.SelectedKeyName, attack_delay);
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_SelectRandomAction::SelectCombatAction(AAZMonster* owner, UBlackboardComponent* blackboard)
{
	if (!Cast<AActor>(blackboard->GetValueAsObject(key_target_character_.SelectedKeyName)))
	{
		return EBTNodeResult::Failed;
	}
	
	TArray<int32> available_action_ids;
	for (auto action_info : owner->combat_action_map_)
	{
		const EMonsterActionTriggerType triggers = action_info.Value.triggers;
		if (triggers != EMonsterActionTriggerType::None)
		{
			//TODO currently no trigger is implemented for combat actions
			continue;
		}
		if (triggers == EMonsterActionTriggerType::Auto)
		{
			continue;
		}
		// Filter out actions that fail the conditions
		const EMonsterActionConditionType conditions = action_info.Value.conditions;
		if ((conditions & EMonsterActionConditionType::Flying) == EMonsterActionConditionType::Flying)
		{
			// Unacceptable if the owner should be flying but is not
			if (!owner->IsFlying()) continue;
		}
		else
		{
			// Unacceptable if the owner should not be flying but is
			if (owner->IsFlying()) continue;
		}
		if ((conditions & EMonsterActionConditionType::InRange) == EMonsterActionConditionType::InRange)
		{
			// Unacceptable if the target player is not in range
			FVector target_location = Cast<AActor>(blackboard->GetValueAsObject(key_target_character_.SelectedKeyName))->GetActorLocation();
			float distance_to_target = owner->GetDistance2DToLocation(target_location);
			if (action_info.Value.condition_min_distance_from_target != -1)
			{
				if (distance_to_target < action_info.Value.condition_min_distance_from_target) continue;
			}
			if (action_info.Value.condition_max_distance_from_target != -1)
			{
				if (distance_to_target > action_info.Value.condition_max_distance_from_target) continue;
			}
		}
		if ((conditions & EMonsterActionConditionType::Health) == EMonsterActionConditionType::Health)
		{
			// Unacceptable if health condition is not met
			//TODO currently not implemented
		}
		//TODO cooltime
		// acceptable if all conditions are met
		available_action_ids.Add(action_info.Key);
	}

	// If there is no available action, return fail
	if (available_action_ids.IsEmpty())
		return EBTNodeResult::Failed;
	
	// Select random action from available actions
	const int idx = FMath::RandRange(0, available_action_ids.Num()-1);
	owner->SetActionState(available_action_ids[idx]);

	// Set action delay
	float attack_delay = owner->combat_action_map_.Find(available_action_ids[idx])->attack_delay;
	blackboard->SetValueAsFloat(key_action_delay_.SelectedKeyName, attack_delay);
	
	return EBTNodeResult::Succeeded;
}
