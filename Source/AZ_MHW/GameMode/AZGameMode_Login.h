// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZGameMode_Login.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZGameMode_Login : public AAZGameMode
{
	GENERATED_BODY()
	
public:
	AAZGameMode_Login();
	virtual void InitGame(const FString& map_name, const FString& options, FString& error_message) override;
	virtual EGameModeType GetGameModeType() { return EGameModeType::Login; }
	virtual void BeginPlay() override;
};
