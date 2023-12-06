// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZItemData.h"
#include "AZWeaponItem.generated.h"

/**
 * 
 */
enum class EWeaponType :uint8;

UCLASS()
class AZ_MHW_API UAZWeaponItem : public UObject
{
	GENERATED_BODY()

private:
	FWeaponInfo info_;
public:
	void InitItem(FWeaponInfo& info);
	EWeaponType GetWeaponType();
	FWeaponInfo GetWeaponInfo();
	bool GetEquipState();
	void EquipStateChange(bool state);
};
