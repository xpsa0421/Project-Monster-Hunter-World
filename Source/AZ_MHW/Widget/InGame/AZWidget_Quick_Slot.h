// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZ_MHW/Widget/InGame/AZWidget_Quick_Slot_Item.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AZWidget_Quick_Slot.generated.h"


/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Quick_Slot : public UAZWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void OnClose(bool immediately = false) override;
	virtual void Update() override;
	virtual void ForceHide() override;
	virtual void RestoreFromForceHidden() override;

public:
	UPROPERTY(meta = (BindWidget)) UAZWidget_Quick_Slot_Item* quick_use_slot_0_;
	UPROPERTY(meta = (BindWidget)) UAZWidget_Quick_Slot_Item* quick_use_slot_1_;
	UPROPERTY(meta = (BindWidget)) UAZWidget_Quick_Slot_Item* quick_use_slot_2_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* quick_slot_count_;
};
