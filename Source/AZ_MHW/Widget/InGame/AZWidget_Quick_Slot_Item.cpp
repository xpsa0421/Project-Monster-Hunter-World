// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/InGame/AZWidget_Quick_Slot_Item.h"

void UAZWidget_Quick_Slot_Item::Init()
{
	item_image_->SetBrushFromTexture(nullptr);
	item_image_->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	item_grade_image_->SetBrushFromTexture(nullptr);
	item_grade_image_->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}
