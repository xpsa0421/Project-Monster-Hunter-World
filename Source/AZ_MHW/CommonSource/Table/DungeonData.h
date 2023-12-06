#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "DungeonData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UDungeonData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 map_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 target_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 area_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 goal_string;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool is_boss;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
