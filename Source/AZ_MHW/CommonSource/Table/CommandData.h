#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "CommandData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UCommandData : public UTableBase
{
	GENERATED_BODY()

public:

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
