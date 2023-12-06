// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayer_Remotable.h"

AAZPlayer_Remotable::AAZPlayer_Remotable()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AAZPlayer_Remotable::BeginPlay()
{
	Super::BeginPlay();
}

void AAZPlayer_Remotable::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZPlayer_Remotable::BeginDestroy()
{
	Super::BeginDestroy();
}


