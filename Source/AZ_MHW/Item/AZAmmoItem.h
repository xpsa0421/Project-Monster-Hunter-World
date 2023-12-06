// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZItemData.h"
#include "AZAmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAmmoItem : public UObject
{
	GENERATED_BODY()
private:
	FAmmoInfo info_;
public:
	void InitItem(FAmmoInfo& item_info);
	void IncreaseCount(int32 count = 1);
	void DecreaseCount(int32 count = 1);
	
	int32 GetItemCount();
	int32 GetItemKey();
	FAmmoInfo GetItemInfo();
	EStorageType GetStorageType();
};
