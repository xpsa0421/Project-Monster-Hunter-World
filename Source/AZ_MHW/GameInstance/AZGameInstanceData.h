// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZGameInstanceData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMapChannelInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY() int32 map_index;
	UPROPERTY() int32 map_channel_index;
	UPROPERTY() int32 map_player_count;

public:
	FMapChannelInfo()
	{
		Reset();
	}
	void Reset()
	{
		map_index = 0;
		map_channel_index = 0;
		map_player_count = 0;
	}
};
