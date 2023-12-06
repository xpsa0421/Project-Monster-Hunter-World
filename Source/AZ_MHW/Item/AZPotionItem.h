// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZItemData.h"
#include "AZPotionItem.generated.h"

/**
 * 
 */

UCLASS()
class AZ_MHW_API UAZPotionItem : public UObject
{
	GENERATED_BODY()

private:
	FPotionInfo info_;
public:
	void			InitItem(FPotionInfo& new_info);
	void			SetCount(int32 count);
	void			IncreaseCount(int32 count =1);
	void			DecreaseCount(int32 count =1);
	FPotionInfo		GetItemInfo() const;
	int32			GetItemCount();
	int32			GetItemKey();
	EStorageType	GetItemStorageType();
};
