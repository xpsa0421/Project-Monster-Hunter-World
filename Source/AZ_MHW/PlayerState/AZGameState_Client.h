// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AZGameState_Client.generated.h"

UCLASS()
class AZ_MHW_API AAZGameState_Client : public AGameStateBase
{
	GENERATED_BODY()

public:
	AAZGameState_Client();

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
public:
};
