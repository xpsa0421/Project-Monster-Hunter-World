// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AZ_MHW/Interface/AZActor_Interface.h"

#include "AZActor.generated.h"

UCLASS()
class AZ_MHW_API AAZActor : public AActor, public IAZActor_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAZActor();
	virtual void Init();

	virtual int32 GetObjectSerial() const override { return INDEX_NONE; }
	virtual int32 GetObjectId() const override { return INDEX_NONE; }
};