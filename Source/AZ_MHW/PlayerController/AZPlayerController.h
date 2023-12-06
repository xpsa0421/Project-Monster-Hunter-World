// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <GameFramework/PlayerController.h>
#include "AZPlayerController.generated.h"

/**
 * 모든 PlayerController Base
 * 
 * Menu(Login)
 * CreateCharacter()
 * Lobby(Town)
 * InGame(Wild)
 */
UCLASS()
class AZ_MHW_API AAZPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAZPlayerController();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void OnPossess(APawn* in_pawn) override;
	/** */
	virtual void SetupInputComponent() override;
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
public:
	/** Capture -> NoneCapture
	 *  NoneCapture -> Capture
	 */
	void ActionChangeControlMode();
	void ActionCloseScene();
	
	//UPROPERTY(BlueprintReadOnly)
	//AAZCameraManager* CameraMgr;
	
	//UPROPERTY(BlueprintReadOnly)
	//AAZPlayerState* State;

	//UPROPERTY(BlueprintReadOnly)
	//AAZHUDManager* HUDMgr;

	//UPROPERTY(BlueprintReadOnly)
	//AAZPlayer* Player;
protected:
	UPROPERTY() class UAZGameInstance* game_instance_;
};
