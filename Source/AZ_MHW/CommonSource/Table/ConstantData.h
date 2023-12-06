#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "ConstantData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UConstantData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 test_int;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString test_string;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
