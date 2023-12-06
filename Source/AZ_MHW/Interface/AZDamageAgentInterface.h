// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "UObject/Interface.h"
#include "AZDamageAgentInterface.generated.h"

UINTERFACE(Blueprintable)
class UAZDamageAgentInterface : public UInterface
{
	GENERATED_BODY()
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamageSignature, AActor*, damage_instigator, const FHitResult, hit_result, FAttackInfo, attack_info);

class AZ_MHW_API IAZDamageAgentInterface
{
	GENERATED_BODY()
	
public:
	FOnTakeDamageSignature OnTakeDamage;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float ApplyDamage(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info);
	
protected:
	// Damage functions
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info);

public:
	virtual float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info);
};
