// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InGamePacket.generated.h"

USTRUCT(BlueprintType)
struct FSetMoveInfo
{
	GENERATED_BODY()

	//UPROPERTY(BlueprintReadWrite)
	uint16 packet_length;

	//UPROPERTY(BlueprintReadWrite)
	uint16 packet_id;

	//UPROPERTY(BlueprintReadWrite)
	FVector fvector_;

	//UPROPERTY(BlueprintReadWrite)
	FRotator frotator_;
};

/**
 * 
 */
UCLASS()
class AZ_MHW_API UInGamePacket : public UObject
{
	GENERATED_BODY()
	
};
