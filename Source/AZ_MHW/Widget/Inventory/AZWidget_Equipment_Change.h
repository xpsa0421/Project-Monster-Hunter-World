 // Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AZWidget_Equipment_Change.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Equipment_Change : public UAZWidget
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAZWidget_Equipment_Slot> equip_slot_;
	UPROPERTY() TArray<UAZWidget_Equipment_Slot*> equip_slot_array_;


	UPROPERTY(meta = (BindWidget)) UAZButton* weapon_button_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* weapon_text_;

	UPROPERTY(meta = (BindWidget)) UAZButton* helmet_button_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* helmet_text_;

	UPROPERTY(meta = (BindWidget)) UAZButton* armor_button_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* armor_text_;

	UPROPERTY(meta = (BindWidget)) UAZButton* glove_button_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* glove_text_;

	UPROPERTY(meta = (BindWidget)) UAZButton* coil_button_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* coil_text_;

	UPROPERTY(meta = (BindWIdget)) UAZButton* greave_button_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* greave_text_;

	UPROPERTY(meta = (BindWIdget)) UUniformGridPanel* equip_inventory_0_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* equip_text_;
	UPROPERTY(meta = (BindWIdget)) UImage* equip_inven_image_;
};
