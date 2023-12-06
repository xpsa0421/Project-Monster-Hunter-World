#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "LoadingData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API ULoadingData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 map_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString model_type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 monster_group_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 npc_group_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 tip_group_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float location_x;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float location_y;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float location_z;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
