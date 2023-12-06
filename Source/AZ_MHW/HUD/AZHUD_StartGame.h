// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZHUD_StartGame.generated.h"

/**
 * 
 */
struct FAZWidgetData;
enum class EUIName : uint16;
UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API AAZHUD_StartGame : public AAZHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason);
};
