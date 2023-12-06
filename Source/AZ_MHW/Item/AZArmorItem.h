// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZItemData.h"

#include "AZArmorItem.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZArmorItem : public UObject
{
	GENERATED_BODY()
private:
	FArmorInfo info_;
public:
	void InitItem(FArmorInfo& info);
	FArmorInfo GetItemInfo();
	EArmorType GetArmorType();
	bool	GetEquipState();
	void	EquipStateChange(bool equip); 
};
