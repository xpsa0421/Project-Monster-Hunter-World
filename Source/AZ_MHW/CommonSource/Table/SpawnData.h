#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "SpawnData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API USpawnData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> map_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 drop_group_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 spawn_count;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float patrol_range;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> obj_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool is_active;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
