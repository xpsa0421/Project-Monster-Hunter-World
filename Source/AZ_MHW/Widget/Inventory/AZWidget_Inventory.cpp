// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Widget/Inventory/AZWidget_Equipment.h"
#include "AZ_MHW/Widget/Inventory/AZWidget_Equipment_Change.h"
#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory_Container.h"
#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory_Change.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/HUD/AZHUD.h"

void UAZWidget_Inventory::Init()
{
	Super::Init();
	AZ_PRINT_LOG_IF_FALSE(item_check_button_);
	item_check_button_->OnClicked.AddDynamic(this, &UAZWidget_Inventory::OnItemCheck);
	AZ_PRINT_LOG_IF_FALSE(item_move_button_);
	item_move_button_->OnClicked.AddDynamic(this, &UAZWidget_Inventory::OnItemMove);
	AZ_PRINT_LOG_IF_FALSE(item_equip_check_button_);
	item_equip_check_button_->OnClicked.AddDynamic(this, &UAZWidget_Inventory::OnEquipCheck);
	AZ_PRINT_LOG_IF_FALSE(item_equip_move_button_);
	item_equip_move_button_->OnClicked.AddDynamic(this, &UAZWidget_Inventory::OnEquipMove);
}

void UAZWidget_Inventory::OnOpen(bool immediately)
{
	// 처음 위젯 생성 될 때
	Super::OnOpen(immediately);
	return;
}

void UAZWidget_Inventory::OnClose(bool immediately)
{
	Super::OnClose(immediately);
	return;
}

void UAZWidget_Inventory::EditorOnly_Refresh()
{
	Super::EditorOnly_Refresh();
}

void UAZWidget_Inventory::Update()
{
	Super::Update();
	return;
}

bool UAZWidget_Inventory::DifferenceBackButton()
{
	return false;
}

void UAZWidget_Inventory::OnTouchEmptySpace()
{
	Super::OnTouchEmptySpace();
}

void UAZWidget_Inventory::OnAlphaTimeFinished()
{
	Super::OnAlphaTimeFinished();
}

void UAZWidget_Inventory::ForceHide()
{
	Super::ForceHide();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_Inventory::RestoreFromForceHidden()
{
	Super::RestoreFromForceHidden();
	SetVisibility(ESlateVisibility::Visible);
}

void UAZWidget_Inventory::OnItemCheck()
{
	item_check_ = game_instance_->GetHUD()->OpenScene<UAZWidget_Inventory_Container>(EUIName::AZWidget_Inventory_Container);
}

void UAZWidget_Inventory::OnItemMove()
{
	item_move_ = game_instance_->GetHUD()->OpenScene<UAZWidget_Inventory_Change>(EUIName::AZWidget_Inventory_Change);
}

void UAZWidget_Inventory::OnEquipCheck()
{
	equip_check_ = game_instance_->GetHUD()->OpenScene<UAZWidget_Equipment>(EUIName::AZWidget_Equipment);
}

void UAZWidget_Inventory::OnEquipMove()
{
	equip_move_ = game_instance_->GetHUD()->OpenScene<UAZWidget_Equipment_Change>(EUIName::AZWidget_Equipment_Change);
}
