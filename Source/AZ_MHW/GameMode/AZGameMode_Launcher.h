// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZGameMode_Launcher.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZGameMode_Launcher : public AAZGameMode
{
	GENERATED_BODY()

public:
	AAZGameMode_Launcher();

	virtual void InitGame(const FString& map_name, const FString& options, FString& error_message) override;
};
