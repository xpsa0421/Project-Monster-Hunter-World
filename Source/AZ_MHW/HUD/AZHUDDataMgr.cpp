// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUDDataMgr.h"

#include "AZ_MHW/Manager/AZResourceMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "Kismet/GameplayStatics.h"
#undef MessageBox
#undef MessageBoxW



void UAZHUDDataMgr::Init()
{
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	widget_datas.Empty();

	// launcher
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Scene_Launcher, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Launcher.BP_Widget_Launcher_C"));
	// black bg
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Login, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Login.BP_Widget_Login_C"));
	// default back bg
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Menu, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Menu.BP_Widget_Menu_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_CharacterCreate, false, false, TEXT("/Game/AZ/Widget/BP_Widget_CharacterCreate.BP_Widget_CharacterCreate_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_CharacterSelect, false, false, TEXT("/Game/AZ/Widget/BP_Widget_CharacterSelect.BP_Widget_CharacterSelect_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Equipment, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Equipment.BP_Widget_Equipment_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Equipment_Change, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Equipment_Change.BP_Widget_Equipment_Change_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Equipment_Slot, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Equipment_Slot.BP_Widget_Equipment_Slot_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_HearthBars, false, false, TEXT("/Game/AZ/Widget/BP_Widget_HearthBars.BP_Widget_HearthBars_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_InGame, false, false, TEXT("/Game/AZ/Widget/BP_Widget_InGame.BP_Widget_InGame_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Inventory, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Inventory.BP_Widget_Inventory_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Inventory_Change, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Inventory_Change.BP_Widget_Inventory_Change_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Inventory_Container, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Inventory_Container.BP_Widget_Inventory_Container_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Inventory_Slot, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Inventory_Slot.BP_Widget_Inventory_Slot_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Minimap, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Minimap.BP_Widget_Minimap_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_PopupDamageInfo, false, false, TEXT("/Game/AZ/Widget/BP_Widget_PopupDamageInfo.BP_Widget_PopupDamageInfo_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Quick_Slot, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Quick_Slot.BP_Widget_Quick_Slot_C"));
	InitWidgetData(EUILayer::Scene, EUIName::AZWidget_Quick_Slot_Item, false, false, TEXT("/Game/AZ/Widget/BP_Widget_Quick_Slot_Item.BP_Widget_Quick_Slot_Item_C"));

	// WaitingWidget
	InitWidgetData(EUILayer::WaitingWidget, EUIName::AZWidget_Waiting, true, false, TEXT("/Game/AZ/Widget/BP_Widget_Waiting.BP_Widget_Waiting_C"));
	// Basic Box
	InitWidgetData(EUILayer::MsgBox, EUIName::AZWidget_MessageBoxBasic, true, false, TEXT("/Game/AZ/Widget/BP_Widget_MsgBox.BP_Widget_MsgBox_C"));


	// InOut
	InitWidgetData(EUILayer::Top, EUIName::AZWidget_BlackInOut, true, false, TEXT(""));

	widget_data_request_states.Empty();
}

void UAZHUDDataMgr::Reset()
{
	widget_data_request_states.Empty();
}

FAZWidgetData* UAZHUDDataMgr::GetWidgetData(EUIName ui_name_enum)
{
	return widget_datas.Find(ui_name_enum);
}

const FAZWidgetData* UAZHUDDataMgr::GetWidgetData(EUIName ui_name_enum) const
{
	return widget_datas.Find(ui_name_enum);
}

FAZWidgetData* UAZHUDDataMgr::GetSubWidgetData(ESubUIName ui_name_enum)
{
	return sub_widget_datas.Find(ui_name_enum);
}

EAZWidgetDataRequestState UAZHUDDataMgr::GetWidgetDataRequestState(EUIName ui_name)
{
	return widget_data_request_states.Contains(ui_name) ? widget_data_request_states[ui_name] : EAZWidgetDataRequestState::Completed;
}

bool UAZHUDDataMgr::RequestWidgetData(EUIName ui_name)
{
	EAZWidgetDataRequestState current_state = GetWidgetDataRequestState(ui_name);
	if (current_state == EAZWidgetDataRequestState::Completed)
	{
		//Data collect state, widget open
		return true;
	}

	if (current_state == EAZWidgetDataRequestState::Requested)
	{
		// Data request state. waiting
		return false;
	}

	// FIXME (socket merged check) data request
	//if (auto* socketHolder = LHGameInstance->GetSocketHolder(ESocketHolderType::Gate))
	//{
	//	switch (uiName)
	//	{
	//	case EUIName::LHWidget_PKMain:
	//	{
	//		FCG_PK_TOTAL_INFO_SYN msg;
	//		socketHolder->SendPacket(&msg);
	//	}
	//	break;
	//	}

	//	if (WidgetDataRequestStates.Contains(uiName))
	//		WidgetDataRequestStates[uiName] = ELHWidgetDataRequestState::Requested;
	//}
	return false;
}

void UAZHUDDataMgr::OnSuccessWidgetDataRequest(EUIName ui_name)
{
	if (widget_data_request_states.Contains(ui_name) && widget_data_request_states[ui_name] == EAZWidgetDataRequestState::Requested)
	{
		widget_data_request_states[ui_name] = EAZWidgetDataRequestState::Completed;
	}
}

void UAZHUDDataMgr::OnFailedWidgetDataRequest(EUIName ui_name)
{
	if (widget_data_request_states.Contains(ui_name) && widget_data_request_states[ui_name] == EAZWidgetDataRequestState::Requested)
	{
		widget_data_request_states[ui_name] = EAZWidgetDataRequestState::None;
	}
}

void UAZHUDDataMgr::InitWidgetData(EUILayer ui_layer, EUIName ui_name, bool is_stay_in_viewport, bool disable_world_rendering, FString widget_path)
{
	FAZWidgetData ui_widget_data;
	ui_widget_data.layer = ui_layer;
	ui_widget_data.is_stay_in_viewport = is_stay_in_viewport;
	ui_widget_data.widget_id = (uint16)ui_name;
	ui_widget_data.widget_full_path = widget_path;
	ui_widget_data.disable_world_rendering = disable_world_rendering;
	widget_datas.Add(ui_name, ui_widget_data);
}

void UAZHUDDataMgr::InitWidgetDataRequestState(EUIName ui_name)
{
	widget_data_request_states.Emplace(ui_name, EAZWidgetDataRequestState::None);
}

UAZWidget* FAZWidgetData::GetWidget()
{
	UAZWidget* widget = widget_ptr.Get();
	return widget;
}

UAZWidget* FAZWidgetData::GetOrCreateWidget(bool& bGetWidget, UAZGameInstance* game_instance)
{
	UAZWidget* widget = widget_ptr.Get();
	if (widget == nullptr)
	{
		UClass* load_class = AZResourceHelper::LoadClassFast<UAZWidget>(widget_full_path);
		widget_ptr = CreateWidget<UAZWidget, UAZGameInstance*>(game_instance, load_class);
		widget = widget_ptr.Get();
		bGetWidget = false;
	}
	else
	{
		bGetWidget = true;
	}

	if (widget == nullptr)
	{
		// FIXME need log
	}
	return widget;
}

bool FAZWidgetData::IsWidgetValid() const
{
	return widget_ptr.IsValid();
}
