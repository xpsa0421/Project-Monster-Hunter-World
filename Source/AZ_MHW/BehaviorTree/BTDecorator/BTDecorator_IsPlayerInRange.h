// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPlayerInRange.generated.h"


UCLASS()
class AZ_MHW_API UBTDecorator_IsPlayerInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsPlayerInRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& owner_comp, uint8* node_memory) const override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

public:
	UPROPERTY(EditAnywhere, Category = "AZ") float percept_radius;
};
