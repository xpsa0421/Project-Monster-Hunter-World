// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameFramework/SaveGame.h>
#include "AZ_MHW/Manager/SaveData/GameOptionSaveData.h"
#include "AZSaveData.generated.h"

/**
 * 
 */
UCLASS(Config = AZCommon, Blueprintable, BlueprintType)
class AZ_MHW_API UOptionSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite) FGameOptionSaveData game_option_save_data;
};

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZSaveData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite) UOptionSaveData* option_save_data;
public:
	UAZSaveData();

	void LoadAll();
	void SaveOptionData();
};
