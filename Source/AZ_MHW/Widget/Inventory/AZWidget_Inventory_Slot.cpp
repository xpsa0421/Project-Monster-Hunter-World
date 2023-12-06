// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory_Slot.h"

void UAZWidget_Inventory_Slot::Init()
{
	item_image_->SetBrushFromTexture(nullptr);
	item_image_->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	item_count_->SetText(FText::FromString(L"0"));
	item_grade_image_->SetBrushFromTexture(nullptr);
	item_grade_image_->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}
