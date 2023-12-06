#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "ItemBuffData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UItemBuffData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString target;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float effect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 duration_time;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString particle_path;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString calculation;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
