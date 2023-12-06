// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "AZWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	AAZWorldSettings();

public:
	UPROPERTY(EditAnywhere, Category = AZ_Platform) TArray<FString> platform_level_name;
	UPROPERTY(EditAnywhere, Category = AZ_Weather) class UParticleSystem* weather_particle_system;
	UPROPERTY(EditAnywhere, Category = AZ_Weather) FVector weather_add_location;
};
