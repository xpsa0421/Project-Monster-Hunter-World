// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZHUD_CharacterSelect.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZHUD_CharacterSelect : public AAZHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason);
	
};
