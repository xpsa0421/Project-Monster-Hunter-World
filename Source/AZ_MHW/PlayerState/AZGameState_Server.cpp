// Copyright Team AZ. All Rights Reserved.


#include "AZGameState_Server.h"


AAZGameState_Server::AAZGameState_Server()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAZGameState_Server::BeginPlay()
{
	Super::BeginPlay();
}

void AAZGameState_Server::BeginDestroy()
{
	Super::BeginDestroy();
}

