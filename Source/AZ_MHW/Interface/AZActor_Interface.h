// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AZActor_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAZActor_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
 // FIXME 추후 Component적용 추가하기
class AZ_MHW_API IAZActor_Interface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetObjectSerial() const { return INDEX_NONE; }
	virtual int32 GetObjectId() const { return INDEX_NONE; }
};
