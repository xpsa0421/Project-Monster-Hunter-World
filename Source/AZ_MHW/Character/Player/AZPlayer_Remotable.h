// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "AZPlayer_Remotable.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayer_Remotable : public AAZPlayer
{
	GENERATED_BODY()
	
public:
	AAZPlayer_Remotable();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void Tick(float delta_seconds) override;
	/** */
	virtual void BeginDestroy() override;
	/** */
	//virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component) override;
	/** */
	//virtual void PossessedBy(AController* new_controller) override;
#pragma endregion
};
