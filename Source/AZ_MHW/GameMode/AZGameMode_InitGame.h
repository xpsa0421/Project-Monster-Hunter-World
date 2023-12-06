// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZGameMode.h"
#include "AZGameMode_InitGame.generated.h"

/**
 * 게임실행시 명령줄 인수로 서버와 클라이언트의 게임모드를 분기합니다.
 */
UCLASS()
class AZ_MHW_API AAZGameMode_InitGame : public AAZGameMode
{
	GENERATED_BODY()

public:
	AAZGameMode_InitGame();

protected:
	virtual void BeginPlay() override;

	virtual void InitGame(const FString& map_name, const FString& options, FString& error_message) override;

	/** 명령줄 인수로 분기실행*/
	UFUNCTION(BlueprintCallable)
	void PlayServerMode();
	/** 명령줄 인수로 분기실행*/
	UFUNCTION(BlueprintCallable)
	void PlayClientMode();
};
