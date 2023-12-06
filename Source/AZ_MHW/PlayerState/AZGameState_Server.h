// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AZGameState_Server.generated.h"

/**
 * 서버 설정용
 */
UCLASS()
class AZ_MHW_API AAZGameState_Server : public AGameStateBase
{
	GENERATED_BODY()

public:
	AAZGameState_Server();

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:
};
