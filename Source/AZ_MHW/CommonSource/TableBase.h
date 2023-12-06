// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TableBase.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UTableBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Load(TMap<FString, FString>& file_path) {};
};
