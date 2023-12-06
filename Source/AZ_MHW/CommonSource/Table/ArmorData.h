#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "ArmorData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UArmorData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 defense;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
