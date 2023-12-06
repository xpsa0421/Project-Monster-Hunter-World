// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AZWorldMap.generated.h"

USTRUCT()
struct FWorldMapDetail
{
	GENERATED_BODY()

public:
	//UPROPERTY() class AAZCameraActor_WorldMap* CameraActor;
	UPROPERTY() class AVolume* AreaVolum;
	UPROPERTY() class ADirectionalLight* AreaDirectionalLight;
	UPROPERTY() class ASkyLight* AreaSkyLight;
	UPROPERTY() class APostProcessVolume* AreaPostProcessVolume;
	UPROPERTY() class AExponentialHeightFog* AreaExponentialHeightFog;
};

/**
 * 
 */
UCLASS(Config = AZCommon)
class AZ_MHW_API UAZWorldMap : public UObject
{
	GENERATED_BODY()

public:
	void Init();

	void ClearWorldMapInfo();

private:
	UPROPERTY() TMap<int32, FWorldMapDetail> world_map_detail_infos_;
};
