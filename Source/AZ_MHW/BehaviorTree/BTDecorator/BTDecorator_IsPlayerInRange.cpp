// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/BehaviorTree/BTDecorator/BTDecorator_IsPlayerInRange.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/Character/Player/AZPlayer_Origin.h"
#include <Kismet/KismetSystemLibrary.h>

#include "Kismet/KismetMathLibrary.h"

UBTDecorator_IsPlayerInRange::UBTDecorator_IsPlayerInRange()
{
	NodeName = TEXT("Is Player In Range");
}

bool UBTDecorator_IsPlayerInRange::CalculateRawConditionValue(UBehaviorTreeComponent& owner_comp, uint8* node_memory) const
{
	if (const auto blackboard = owner_comp.GetBlackboardComponent())
	{
		float check_radius = percept_radius;
		if (check_radius == 0.0f)
		{
			check_radius = Cast<AAZAIController>(owner_comp.GetAIOwner())->percept_radius_;
			if (check_radius <= 0) return false;
		}

		// Check surroundings for a player
		FVector my_loc = owner_comp.GetAIOwner()->GetPawn()->GetActorLocation();
		TArray<AActor*> actors_in_range;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), my_loc, check_radius,
			TArray<TEnumAsByte<EObjectTypeQuery>>(), AAZPlayer_Origin::StaticClass(), TArray<AActor*>(), actors_in_range);
		return !actors_in_range.IsEmpty();
	}
	return false;
}

#if WITH_EDITOR

FName UBTDecorator_IsPlayerInRange::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.ConeCheck.Icon");
}

#endif	// WITH_EDITOR

