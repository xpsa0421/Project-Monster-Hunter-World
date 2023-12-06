// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "AZPlayer_Playable.generated.h"


class AAZPlayerState_Client;


/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayer_Playable : public AAZPlayer
{
	GENERATED_BODY()

public:
	AAZPlayer_Playable();
	
#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void Tick(float delta_seconds) override;
	/** */
	virtual void BeginDestroy() override;
	/** */
	virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component) override;
	/** */
	virtual void PossessedBy(AController* new_controller) override;
#pragma endregion
#pragma region Animation Notify
public:
	UFUNCTION() void AnimNotify_OnUseItem();
	
	UFUNCTION() void AnimNotify_OnGetItem();
	int32 character_index_;
#pragma endregion 
};