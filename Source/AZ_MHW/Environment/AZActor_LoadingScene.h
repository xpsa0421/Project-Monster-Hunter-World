// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AZ_MHW/Interface/AZMesh_Interface.h"

#include "AZActor_LoadingScene.generated.h"

//FIXME merged check loading
UCLASS()
class AZ_MHW_API AAZActor_LoadingScene : public AActor, public IAZMesh_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAZActor_LoadingScene();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCaptureCameraLocation();
	void SetMergeCompVisible(bool merge_visibility);
	void StopCapture();
};
