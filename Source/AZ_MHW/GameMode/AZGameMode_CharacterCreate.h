// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZGameMode_CharacterCreate.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZGameMode_CharacterCreate : public AAZGameMode
{
	GENERATED_BODY()
	
public:
	AAZGameMode_CharacterCreate();
	virtual void InitGame(const FString& map_name, const FString& options, FString& error_message) override;
	virtual EGameModeType GetGameModeType() { return EGameModeType::CharacterCreate; }
	virtual void BeginPlay() override;
};
