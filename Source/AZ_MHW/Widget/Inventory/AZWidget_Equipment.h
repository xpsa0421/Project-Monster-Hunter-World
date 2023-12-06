// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "AZWidget_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Equipment : public UAZWidget
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
	void DisplaySampleSlots(int display_slot_count = 25);
	UUniformGridPanel* GetUniformGridFromPanel(int32 panel_slot);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAZWidget_Equipment_Slot> equip_slot_;
	UPROPERTY() TArray<UAZWidget_Equipment_Slot*> equip_slot_array_;

	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* equip_inventory_0_;
	UPROPERTY(meta = (BindWIdget)) UUniformGridPanel* equip_inventory_1_;
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* equip_inventory_2_;

	UPROPERTY(meta = (BindWidget)) UTextBlock* equip_title_text_;
	UPROPERTY(meta = (BindWidget)) UImage* equip_title_image_;

	UPROPERTY(meta = (BindWidget)) UAZButton* prev_page_button_;
	UPROPERTY(meta = (BindWIdget)) UTextBlock* current_page_text_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* total_page_text_;
	UPROPERTY(meta = (BindWidget)) UAZButton* next_page_button_;

	UPROPERTY(meta = (BindWidget)) UImage* equip_sub_image_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* equip_sub_text_;

	UPROPERTY(meta = (BindWidget)) UTextBlock* equip_coin_text_;

	UPROPERTY(meta = (BindWidget)) UTextBlock* equip_total_count_;

};
