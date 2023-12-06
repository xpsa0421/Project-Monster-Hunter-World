// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Inventory/AZWidget_Equipment.h"

#include "AZ_MHW/Widget/Inventory/AZWidget_Equipment_Slot.h"
#include "Components/UniformGridSlot.h"

void UAZWidget_Equipment::Init()
{
	Super::Init();
}

void UAZWidget_Equipment::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	DisplaySampleSlots();
}

void UAZWidget_Equipment::OnClose(bool immediately)
{
	Super::OnClose(immediately);
	equip_slot_array_.Empty();
}

void UAZWidget_Equipment::Update()
{
	Super::Update();
}

void UAZWidget_Equipment::ForceHide()
{
	Super::ForceHide();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_Equipment::RestoreFromForceHidden()
{
	Super::RestoreFromForceHidden();
	SetVisibility(ESlateVisibility::Visible);
}

void UAZWidget_Equipment::DisplaySampleSlots(int display_slot_count)
{
	int row_lenght = 5;
	for (int i = 0; i < 3; ++i)
	{
		UUniformGridPanel* grid_panel = GetUniformGridFromPanel(i);

		grid_panel->ClearChildren();
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
			auto grid_slot = grid_panel->AddChildToUniformGrid(slot, row, column);
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
}

UUniformGridPanel* UAZWidget_Equipment::GetUniformGridFromPanel(int32 panel_slot)
{
	UUniformGridPanel* grid_panel = nullptr;
	if (panel_slot == 0)
	{
		grid_panel = equip_inventory_0_;
	}
	else if (panel_slot == 1)
	{
		grid_panel = equip_inventory_1_;
	}
	else if (panel_slot == 2)
	{
		grid_panel = equip_inventory_2_;
	}
	return grid_panel;
}
