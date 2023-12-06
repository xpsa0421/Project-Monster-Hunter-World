#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "MapData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UMapData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString map_bg;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString minimap_image;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString map_3d_model;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString map_type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 max_user;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 target_index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString map_name;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
