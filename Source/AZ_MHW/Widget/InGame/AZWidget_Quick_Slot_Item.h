// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "Components/Image.h"
#include "AZWidget_Quick_Slot_Item.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Quick_Slot_Item : public UAZWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

public:
	UPROPERTY(meta = (BindWidget)) UImage* item_image_;
	UPROPERTY(meta = (BindWidget)) UImage* item_grade_image_;
};
