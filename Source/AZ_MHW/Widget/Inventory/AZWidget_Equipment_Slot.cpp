// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Inventory/AZWidget_Equipment_Slot.h"

void UAZWidget_Equipment_Slot::Init()
{
	image_equip_->SetBrushFromTexture(nullptr);
	image_equip_->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}
