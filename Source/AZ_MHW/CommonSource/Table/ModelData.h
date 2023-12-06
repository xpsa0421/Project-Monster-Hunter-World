#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "ModelData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UModelData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString model_type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 model_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 model_group_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> model_bp;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
