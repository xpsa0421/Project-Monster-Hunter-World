// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AZWidget_Inventory_Slot.generated.h"


/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Inventory_Slot : public UAZWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;

public:
	UPROPERTY(meta = (BindWidget)) UAZButton* item_button_;
	UPROPERTY(meta = (BindWidget)) UImage* item_image_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* item_count_;
	UPROPERTY(meta = (BindWidget)) UImage* item_grade_image_;
};
