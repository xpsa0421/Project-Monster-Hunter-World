// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Environment/AZActor_LoadingScene.h"

// Sets default values
AAZActor_LoadingScene::AAZActor_LoadingScene()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAZActor_LoadingScene::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAZActor_LoadingScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAZActor_LoadingScene::SetCaptureCameraLocation()
{
	//FIXMe merged check loading
}

void AAZActor_LoadingScene::SetMergeCompVisible(bool merge_visibility)
{
	//FIXME merged check loading
}

void AAZActor_LoadingScene::StopCapture()
{
	//FIXME merged check loading
}

