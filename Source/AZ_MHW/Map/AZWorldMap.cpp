// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Map/AZWorldMap.h"

#include <Engine/ExponentialHeightFog.h>
#include <Engine/DirectionalLight.h>
#include <Engine/SkyLight.h>
#include <Engine/PostProcessVolume.h>
#include <Components/LightComponent.h>
#include <Components/SkyLightComponent.h>
#include <Engine/BlockingVolume.h>
#include <LevelSequenceActor.h>
#include <Engine/World.h>
#include <DefaultLevelSequenceInstanceData.h>
#include <Sections/MovieScene3DTransformSection.h>
#include <Sections/MovieSceneCinematicShotSection.h>
#include <Tracks/MovieSceneCinematicShotTrack.h>
#include <Sections/MovieSceneSpawnSection.h>
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <MovieSceneSequencePlayer.h>

void UAZWorldMap::Init()
{
}

void UAZWorldMap::ClearWorldMapInfo()
{
	world_map_detail_infos_.Empty();
}
