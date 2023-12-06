// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/Define/MapStruct.h"
#include "AZMapStatics.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZMapStatics : public UObject
{
	GENERATED_BODY()

public:
	static TArray<FSpawnObjectInfo> GetCurMapSpawnObjectInfo();
	static TArray<FSpawnObjectInfo> GetMapSpawnObjectInfo(int32 map_index);
};
