#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "BossData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UBossData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 boss_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 monster_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString rank;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 base_stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 tired_duration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool has_transition_animation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 num_allowed_escapes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> escape_health_ratios;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> weakness_head;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> weakness_neck;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> weakness_body;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> weakness_tail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> weakness_wing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> weakness_leg;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> damage_head;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> damage_body;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> damage_wing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> damage_tail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> damage_leg;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> stunnable_parts;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 rage_required_damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 rage_duration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 rage_agility_multiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 rage_outgoing_damage_multiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 rage_incoming_damage_multiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 tenderised_damage_multiplier;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
