#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "BottleData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UBottleData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool is_buff;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
