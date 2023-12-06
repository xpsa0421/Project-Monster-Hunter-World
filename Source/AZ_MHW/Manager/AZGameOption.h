// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/Manager/SaveData/GameOptionSaveData.h"
#include "AZGameOption.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameOption : public UObject
{
	GENERATED_BODY()
public:
	static FGameOptionSaveData* option_save_data;

public:
	void Init();

	// FIXME call in AZHUD_Lancher
	void InitGameOption();
};
