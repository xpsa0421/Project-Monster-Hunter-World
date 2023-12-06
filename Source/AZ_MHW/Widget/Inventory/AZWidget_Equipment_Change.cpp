// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Inventory/AZWidget_Equipment_Change.h"

#include "AZ_MHW/Widget/Inventory/AZWidget_Equipment_Slot.h"
#include "Components/UniformGridSlot.h"

void UAZWidget_Equipment_Change::Init()
{
	Super::Init();
}

void UAZWidget_Equipment_Change::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	DisplaySampleSlots();
}

void UAZWidget_Equipment_Change::OnClose(bool immediately)
{
	Super::OnClose(immediately);
	equip_slot_array_.Empty();
}

void UAZWidget_Equipment_Change::Update()
{
	Super::Update();
}

void UAZWidget_Equipment_Change::ForceHide()
{
	Super::ForceHide();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_Equipment_Change::RestoreFromForceHidden()
{
	Super::RestoreFromForceHidden();
	SetVisibility(ESlateVisibility::Visible);
}

void UAZWidget_Equipment_Change::DisplaySampleSlots(int display_slot_count)
{
	int row_lenght = 5;

	equip_inventory_0_->ClearChildren();
	int row = 0;
	int column = 0;
	for (int j = 0; j < display_slot_count; ++j)
	{
		UAZWidget_Equipment_Slot* slot = CreateWidget<UAZWidget_Equipment_Slot>(this, equip_slot_);
		if (slot == nullptr)
		{
			continue;
		}

		// 기본 셋팅
		//...
		slot->Init();
		equip_slot_array_.Add(slot);
		auto grid_slot = equip_inventory_0_->AddChildToUniformGrid(slot, row, column);
		grid_slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		grid_slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

		column++;
		if (column >= row_lenght)
		{
			column = 0;
			++row;
		}
	}
}
