// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "AZWidget_Inventory_Change.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Inventory_Change : public UAZWidget
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
	void DisplaySampleSlots(int display_slot_count = 50);
	UUniformGridPanel* GetUniformGridFromPanel(int32 panel_slot);

	void DisplayItemPocketSlot(int display_slot_count = 24);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAZWidget_Inventory_Slot> item_slot_;

	UPROPERTY() TArray<class UAZWidget_Inventory_Slot*> item_slot_array_;
	UPROPERTY() TArray<class UAZWidget_Inventory_Slot*> item_pocket_slot_array_;

	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* inventory_0_;
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* inventory_1_;
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* inventory_2_;

	UPROPERTY(meta = (BindWidget)) UButton* prev_inven_button_;
	UPROPERTY(meta = (BindWidget)) UButton* next_inven_button_;

	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* pocket_inventory_;

	UPROPERTY(meta = (BindWidget)) UTextBlock* in_count_text_;
	UPROPERTY(meta = (BindWidget)) UButton* up_button_;
	UPROPERTY(meta = (BindWidget)) UButton* down_button_;

	UPROPERTY(meta = (BindWidget)) UImage* in_out_image_;
	UPROPERTY(meta = (BindWidget)) UAZButton* in_out_check_button_;

	UPROPERTY(meta = (BindWidget)) UTextBlock* item_current_name_;
	UPROPERTY(meta = (BindWidget)) UImage* item_current_image_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_current_detail_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_text_grade_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_pocket_current_count_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_pocket_max_count_;
	UPROPERTY(meta = (BindWIdget)) UTextBlock* item_sell_coin_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_slot_max_count_;
};
