#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "MeleeWeaponData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UMeleeWeaponData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 hone;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 critical;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
