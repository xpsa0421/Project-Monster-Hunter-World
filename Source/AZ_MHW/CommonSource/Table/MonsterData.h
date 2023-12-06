#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "MonsterData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UMonsterData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 monster_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString behavior_type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 base_hp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 sight_radius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 sight_lose_radius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 sight_fov;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 sight_max_age;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 sight_auto_success_range;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 patrol_range;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 patrol_delay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 percept_radius;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
