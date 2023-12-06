#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "MonsterCombatActionData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UMonsterCombatActionData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 action_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 monster_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString animation_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString montage_section_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString effect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> damage_types;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> damage_amounts;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> triggers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> conditions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 condition_min_distance_from_target;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 condition_max_distance_from_target;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 attack_delay;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
