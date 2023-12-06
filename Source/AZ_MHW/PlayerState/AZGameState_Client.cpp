// Copyright Team AZ. All Rights Reserved.


#include "AZGameState_Client.h"


AAZGameState_Client::AAZGameState_Client()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAZGameState_Client::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZGameState_Client::BeginDestroy()
{
	Super::BeginDestroy();
}


