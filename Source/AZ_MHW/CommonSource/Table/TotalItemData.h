#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "TotalItemData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UTotalItemData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString value;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 warehouse_max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 pocket_max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 init_count;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString kor_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString image_path;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<float> color;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString grade_image_path;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
