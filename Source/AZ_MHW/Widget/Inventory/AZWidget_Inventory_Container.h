// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AZWidget_Inventory_Container.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Inventory_Container : public UAZWidget
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
	void SetInventory();
	UUniformGridPanel* GetUniformGridFromPanel(int32 panel_slot);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAZWidget_Inventory_Slot> item_slot_;

	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* inventory_0_;
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* inventory_1_;
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* inventory_2_;

	UPROPERTY(meta = (BindWidget)) UTextBlock* item_slot_current_count_; // 1
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_slot_max_; // 3

	UPROPERTY(meta = (BindWidget)) UTextBlock* item_current_name_; // 회복약

	UPROPERTY(meta = (BindWidget)) UImage* item_image_; // 아이템 이미지

	UPROPERTY(meta = (BindWidget)) UTextBlock* item_current_detail_; // 약간의 체력을 회복해 준다.
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_text_grade_; // RARE1

	UPROPERTY(meta = (BindWidget)) UTextBlock* item_slot_text_count_; // 10; 가방에 있는 갯수
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_slot_max_count_; // 10; 가방 넣을수 있는 갯수

	UPROPERTY(meta = (BindWidget)) UTextBlock* item_sell_coin_; // 2 매각액

	UPROPERTY(meta = (BindWidget)) UTextBlock* item_inventory_count_; // 인벤토리 갯수

	UPROPERTY() TArray<class UAZWidget_Inventory_Slot*> item_slot_array_;

public:
	UFUNCTION() void OnItemHover();
};
