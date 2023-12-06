// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory_Container.h"

#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory_Slot.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZInventoryManager.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
#include "PaperSprite.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/UniformGridSlot.h"
#include "AZ_MHW/HUD/AZHUD.h"

void UAZWidget_Inventory_Container::Init()
{
	Super::Init();
}

void UAZWidget_Inventory_Container::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	DisplaySampleSlots();
	SetInventory();
}

void UAZWidget_Inventory_Container::OnClose(bool immediately)
{
	Super::OnClose(immediately);
	item_slot_array_.Empty();
}

void UAZWidget_Inventory_Container::Update()
{
	Super::Update();
}

void UAZWidget_Inventory_Container::ForceHide()
{
	Super::ForceHide();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_Inventory_Container::RestoreFromForceHidden()
{
	Super::RestoreFromForceHidden();
	SetVisibility(ESlateVisibility::Visible);
}

void UAZWidget_Inventory_Container::DisplaySampleSlots(int display_slot_count)
{
	int row_lenght = 10;
	for (int i = 0; i < 3; ++i)
	{
		UUniformGridPanel* grid_panel = GetUniformGridFromPanel(i);

		grid_panel->ClearChildren();
		int row = 0;
		int column = 0;
		for (int j = 0; j < display_slot_count; ++j)
		{
			UAZWidget_Inventory_Slot* slot = CreateWidget<UAZWidget_Inventory_Slot>(this, item_slot_);
			if (slot == nullptr)
			{
				continue;
			}

			// 기본 셋팅
			//...
			slot->Init();
			item_slot_array_.Add(slot);
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
void UAZWidget_Inventory_Container::SetInventory()
{
	auto inven_info = game_instance_->game_cache_info_->GetCurrentCharacterItem();
	auto& potion_slot = inven_info->potion_warehouse_;
	auto& pocket_slot = inven_info->potion_pocket_;
	int i = 0;
	for (auto& slot : pocket_slot)
	{
		auto table_data = GetTableByIndex(UTotalItemData, slot.Value->GetItemKey());
		UTexture2D* texture2d = LoadObject<UTexture2D>(nullptr, *table_data->image_path);
		item_slot_array_[i]->item_image_->SetBrushFromTexture(texture2d);
		int count = 0;
		FLinearColor color;
		for (auto value : table_data->color)
		{
			if (count == 0)
			{
				color.R = value;
			}
			if (count == 1)
			{
				color.G = value;
			}
			if (count == 2)
			{
				color.B = value;
			}
			if (count == 3)
			{
				color.A = value;
			}
			++count;
		}
		item_slot_array_[i]->item_image_->SetColorAndOpacity(color);
		item_image_->SetBrushFromTexture(texture2d);
		item_image_->SetColorAndOpacity(color);
		FString string_count = FString::FromInt(slot.Value->GetItemCount());
		item_inventory_count_->SetText(FText::FromString(string_count));
		item_slot_array_[i]->item_count_->SetText(FText::FromString(string_count));
		item_sell_coin_->SetText(FText::FromString(L"100"));
		item_slot_max_count_->SetText(FText::FromString(L"15"));
		auto pocket_item = pocket_slot[slot.Value->GetItemKey()];
		FString string_pocket_count = FString::FromInt(pocket_item->GetItemCount());
		item_slot_text_count_->SetText(FText::FromString(string_pocket_count));
		FString detail = table_data->description;
		FString name = table_data->kor_name;
		item_current_detail_->SetText(FText::FromString(detail));
		item_current_name_->SetText(FText::FromString(name));
		++i;
	}
}

UUniformGridPanel* UAZWidget_Inventory_Container::GetUniformGridFromPanel(int32 panel_slot)
{
	UUniformGridPanel* grid_panel = nullptr;
	if (panel_slot == 0)
	{
		grid_panel = inventory_0_;
	}
	else if (panel_slot == 1)
	{
		grid_panel = inventory_1_;
	}
	else if (panel_slot == 2)
	{
		grid_panel = inventory_2_;
	}
	return grid_panel;
}

void UAZWidget_Inventory_Container::OnItemHover()
{
}
