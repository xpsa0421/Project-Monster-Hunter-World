// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AZMsgObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAZMsgObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AZ_MHW_API IAZMsgObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnGameMsg(struct FAZGameMsg* msg) {}
};
