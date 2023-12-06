// Copyright Team AZ. All Rights Reserved.


#include "AZPlayer_Playable.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"

AAZPlayer_Playable::AAZPlayer_Playable()
{
	PrimaryActorTick.bStartWithTickEnabled = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAZPlayer_Playable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZPlayer_Playable::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZPlayer_Playable::BeginDestroy()
{
	Super::BeginDestroy();
}


void AAZPlayer_Playable::SetupPlayerInputComponent(UInputComponent* player_input_component)
{
	Super::SetupPlayerInputComponent(player_input_component);
	
}

void AAZPlayer_Playable::PossessedBy(AController* new_controller)
{
	Super::PossessedBy(new_controller);
}

void AAZPlayer_Playable::AnimNotify_OnUseItem()
{
}

void AAZPlayer_Playable::AnimNotify_OnGetItem()
{
}

