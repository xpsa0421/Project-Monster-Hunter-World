// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUD.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZ_MHW/Widget/AZWidget_Waiting.h"
#include "AZ_MHW/Widget/Common/AZWidget_Fade.h"
#include "AZ_MHW/Widget/MsgBox/AZWidget_MsgBoxBase.h"
#include "AZ_MHW/Widget/MsgBox/AZWidget_MsgBoxBasic.h"
#include "AZ_MHW/Manager/AZResourceMgr.h"
#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "Blueprint/WidgetLayoutLibrary.h"

const FVector2D invalid_touch_position(-1.0f, -1.0f);

void AAZHUD::PostLoad()
{
	Super::PostLoad();
}

void AAZHUD::BeginPlay()
{
	Super::BeginPlay();
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	// click effect pool processing
}

void AAZHUD::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);

	// Effect pool processing
	// ...

	CloseAllUI();
}

void AAZHUD::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
	ResetHandledTouchPosition();

	if (0 < msg_box_infos_.Num())
	{
		for (auto& msg_box_info : msg_box_infos_)
		{
			OpenMsgBox(msg_box_info.msg_box_type
				, msg_box_info.desc
				, msg_box_info.btn_type
				, msg_box_info.owner
				, msg_box_info.left_tap_function_name
				, msg_box_info.right_tap_function_name
				, msg_box_info.exit_function_name
				, msg_box_info.left_btn_text
				, msg_box_info.right_btn_text
				, msg_box_info.is_add_wait_msg_box_stack
				, msg_box_info.is_disable_back_btn_exit);
		}
		msg_box_infos_.Empty();
	}
}

void AAZHUD::OnTouchEmptySpace()
{
	if (IsCurScene(cur_scene_name_enum))
	{
		if (UAZWidget* ui = GetUI<UAZWidget>(cur_scene_name_enum))
		{
			ui->OnTouchEmptySpace();
		}
	}
}

void AAZHUD::InitHUD()
{
}

AAZGameMode* AAZHUD::GetGameMode()
{
	return nullptr;
}

void AAZHUD::OnFadeInOut(const float in_time, const float out_time)
{
	if (FAZWidgetData* const widget_data = GetWidgetData(EUIName::AZWidget_Fade))
	{
		bool get_widget = false;
		if (auto widget = Cast<UAZWidget_Fade>(widget_data->GetOrCreateWidget(get_widget, game_instance_)))
		{
			if (widget->IsInViewport() == false)
			{
				widget->AddToViewport((int32)widget_data->layer);

				if (in_time >= 0.0f && out_time < 0.0f)
				{
					widget->FadeIn(in_time);
				}
				else if (in_time < 0.0f && out_time >= 0.0f)
				{
					widget->FadeOut(out_time);
				}
				else if (in_time >= 0.0f && out_time >= 0.0f)
				{
					widget->FadeInOut(in_time, out_time);
				}
			}
		}
	}
}

void AAZHUD::ResetHandledTouchPosition()
{
	handled_touch_position_ = invalid_touch_position;
}

void AAZHUD::OnSceneOpened()
{
	// FIXME Input change on scene change
	//if (AZGameInstance && AZGameInstance->InputMgr)
	//{
	//    AZGameInstance->InputMgr->CheckSceneStack();
	//}
}

void AAZHUD::OnSceneClosed()
{
	// FIXME If it is among the widgets, delete it.
}

bool AAZHUD::GetHandledTouchPosition(FVector2D& position, bool try_get_touch_state) const
{
	if ((handled_touch_position_ - invalid_touch_position).IsNearlyZero())
	{
		if (try_get_touch_state)
		{
			bool bIsCurrentlyPressed;
			GetOwningPlayerController()->GetInputTouchState(ETouchIndex::Touch1, position.X, position.Y, bIsCurrentlyPressed);
			return position.IsNearlyZero() ? false : true;
		}
		return false;
	}

	position = handled_touch_position_;
	return true;
}

void AAZHUD::SetHandledTouchPosition(const FPointerEvent& pointer)
{
	if (pointer.IsTouchEvent())
	{
		handled_touch_position_ = UWidgetLayoutLibrary::GetViewportWidgetGeometry(this).AbsoluteToLocal(pointer.GetScreenSpacePosition());
	}
}

void AAZHUD::CloseAllMsgBox()
{
	for (int32 i = msg_box_stack_.Num() - 1; i >= 0; --i)
	{
		UAZWidget_MsgBoxBase* msg_box = msg_box_stack_.Top();
		if (msg_box)
		{
			CloseMsgBox(msg_box->GetName());
		}
	}

	msg_box_stack_.Empty();
}

void AAZHUD::CloseAllMsgBoxExceptOvertop()
{
	for (int32 i = msg_box_stack_.Num() - 1; i >= 0; --i)
	{
		UAZWidget_MsgBoxBase* msg_box = msg_box_stack_.Top();
		if (msg_box && msg_box->GetMsgBoxType() != EUIMsgBoxType::OvertopBasic)
		{
			CloseMsgBox(msg_box->GetName());
		}
	}
}

UAZWidget_MsgBoxBasic* AAZHUD::OpenMoveToCashShopMsgBox()
{
	UAZWidget_MsgBoxBase* msg_box = game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic,
		"Move",
		EUIMsgBoxBtnType::OkOrCancel, nullptr, TEXT(""), TEXT(""), TEXT(""),
		"Cancle");

	UAZWidget_MsgBoxBasic* msg_box_basic = Cast<UAZWidget_MsgBoxBasic>(msg_box);

	if (msg_box_basic)
	{
		msg_box_basic->SetTitle("상점 구매");
		msg_box_basic->AddHandler(EMsgEventButtonType::Left, this, FName(TEXT("MoveToCashShop")));
	}

	return msg_box_basic;
}

void AAZHUD::MoveToCashShop()
{
	//FIXME 상점 구매 구현
}

UAZWidget_MsgBoxBase* AAZHUD::OpenMsgBox(EUIMsgBoxType msgBoxType, const FString& desc, EUIMsgBoxBtnType btnType, UObject* owner, const FName& leftTapFunctionName, const FName& rightTapFunctionName, const FName& exitFunctionName, const FString& leftBtnText, const FString& rightBtnText, bool isAddWaitMsgBoxStack, bool isDisableBackBtnExit)
{
	EUIName ui_name = EUIName::None;
	switch (msgBoxType)
	{
	case EUIMsgBoxType::Basic:
		ui_name = EUIName::AZWidget_MessageBoxBasic;
		break;
	}

	// 로딩중
	if (game_instance_->map_mgr->IsOnLoading())
	{
		AZ_LOG("Message : %s", *desc);
	}

	auto widget_data = GetWidgetData(ui_name);
	if (!widget_data)
	{
		AZ_PRINT_LOG_IF_FALSE(widget_data, nullptr);
		// 유아이가  Data가 없다. 파일과 경로 확인 
		return nullptr;
	}

	// 메시지 박스는 여러개 만들 수 있도록 새로 생성
	UClass* load_class = AZResourceHelper::LoadClassFast<UAZWidget>(widget_data->widget_full_path);
	AZ_PRINT_LOG_IF_FALSE(load_class, nullptr);
	UAZWidget_MsgBoxBase* msg_box = CreateWidget<UAZWidget_MsgBoxBase, UAZGameInstance*>(game_instance_, load_class);

	if (!msg_box)
	{
		AZ_PRINT_LOG_IF_FALSE(msg_box, nullptr);

		// 생성 실패
		return nullptr;
	}

	msg_box->SetWidgetNameEnum(ui_name);
	msg_box->Init();
	msg_box->AddToViewport((int32)widget_data->layer);
	msg_box->OnOpen();
	msg_box->SetMsgBoxType(msgBoxType);
	msg_box->SetDesc(desc);
	msg_box->SetIsDisableBackBtnExit(isDisableBackBtnExit);

	auto MsgBoxBasic = Cast<UAZWidget_MsgBoxBasic>(msg_box);
	if (MsgBoxBasic)
	{
		MsgBoxBasic->SetButtonType(btnType);
		//if (!leftTapFunctionName.IsNone() && owner->FindFunction(leftTapFunctionName))
		if (!leftTapFunctionName.IsNone())
			MsgBoxBasic->AddHandler(EMsgEventButtonType::Left, owner, leftTapFunctionName);
		if (!rightTapFunctionName.IsNone())
			MsgBoxBasic->AddHandler(EMsgEventButtonType::Right, owner, rightTapFunctionName);
		if (!exitFunctionName.IsNone())
		{
			MsgBoxBasic->AddHandler(EMsgEventButtonType::Cancle, owner, exitFunctionName);
			MsgBoxBasic->AddHandler(EMsgEventButtonType::Close, owner, exitFunctionName);
		}
		if (!leftBtnText.IsEmpty())
			MsgBoxBasic->SetButtonText(EMsgEventButtonType::Left, leftBtnText);
		if (!rightBtnText.IsEmpty())
			MsgBoxBasic->SetButtonText(EMsgEventButtonType::Right, rightBtnText);
	}

	msg_box_stack_.Add(msg_box);

	return msg_box;
}

void AAZHUD::OpenMsgBox_SafeThread(EUIMsgBoxType msgBoxType, const FString& desc, EUIMsgBoxBtnType btnType, UObject* owner, const FName& leftTapFunctionName, const FName& rightTapFunctionName, const FName& exitFunctionName, const FString& leftBtnText, const FString& rightBtnText, bool isAddWaitMsgBoxStack, bool IsDisableBackBtnExit)
{
	FMsgBoxInfo MsgBoxInfo;
	MsgBoxInfo.msg_box_type = msgBoxType;
	MsgBoxInfo.desc = desc;
	MsgBoxInfo.btn_type = btnType;
	MsgBoxInfo.owner = owner;
	MsgBoxInfo.left_tap_function_name = leftTapFunctionName;
	MsgBoxInfo.right_tap_function_name = rightTapFunctionName;
	MsgBoxInfo.exit_function_name = exitFunctionName;
	MsgBoxInfo.left_btn_text = leftBtnText;
	MsgBoxInfo.right_btn_text = rightBtnText;
	MsgBoxInfo.is_add_wait_msg_box_stack = isAddWaitMsgBoxStack;
	MsgBoxInfo.is_disable_back_btn_exit = true;
	msg_box_infos_.Emplace(MsgBoxInfo);
}

void AAZHUD::OpenMsgBox(FString Title, FString Desc)
{
	if (UAZWidget_MsgBoxBase* msg_box = OpenMsgBox(EUIMsgBoxType::Basic, Desc, EUIMsgBoxBtnType::Confirm))
	{
		msg_box->SetTitle(Title);
	}
}

void AAZHUD::OpenMsgBox(FString Desc)
{
	OpenMsgBox(EUIMsgBoxType::Basic, Desc, EUIMsgBoxBtnType::Confirm);
}

void AAZHUD::OpenContentsMsgBox(FString Desc)
{
	OpenMsgBox(EUIMsgBoxType::ContentsBasic, Desc, EUIMsgBoxBtnType::Confirm);
}

void AAZHUD::OpenMsgBox_Confirm(const FString& title, const FString& desc, UObject* owner, const FName& leftTapFunctionName)
{
	if (UAZWidget_MsgBoxBase* msg_box = OpenMsgBox(EUIMsgBoxType::Basic, desc, EUIMsgBoxBtnType::Confirm))
	{
		msg_box->SetTitle(title);
		if (UAZWidget_MsgBoxBasic* msgBoxBasic = Cast<UAZWidget_MsgBoxBasic>(msg_box))
		{
			msgBoxBasic->AddHandler(EMsgEventButtonType::Left, owner, leftTapFunctionName);
		}
	}
}

void AAZHUD::CloseMsgBox(EUIMsgBoxType uiMsgBoxType)
{
	if (msg_box_stack_.Num() == 0)
		return;

	for (int32 i = msg_box_stack_.Num() - 1; 0 <= i; --i)
	{
		if (msg_box_stack_[i]->GetMsgBoxType() == uiMsgBoxType)
		{
			msg_box_stack_[i]->OnClose();
			msg_box_stack_.RemoveAt(i);
			break;
		}
	}

	if (msg_box_stack_.Num() == 0)
	{
		if (AZSceneData* cur_scene_data = GetSceneData(cur_scene_name_enum))
		{
			if (0 < cur_scene_data->child_widget_names.Num())
			{
				EUIName lastUIName = cur_scene_data->child_widget_names.Last();
				if (UAZWidget* lastWidget = GetUI<UAZWidget>(lastUIName))
				{
					if (lastWidget->bIsFocusable)
					{
						lastWidget->SetFocus();
					}
				}
			}
		}
	}
}

void AAZHUD::CloseMsgBox(FString Name)
{
	if (msg_box_stack_.Num() == 0)
	{
		return;
	}

	for (int32 i = msg_box_stack_.Num() - 1; 0 <= i; --i)
	{
		if (msg_box_stack_[i]->GetName() == Name)
		{
			msg_box_stack_[i]->OnClose();
			msg_box_stack_.RemoveAt(i);
			break;
		}
	}

	if (msg_box_stack_.Num() == 0)
	{
		if (AZSceneData* CurSceneData = GetSceneData(cur_scene_name_enum))
		{
			if (0 < CurSceneData->child_widget_names.Num())
			{
				EUIName last_ui_name = CurSceneData->child_widget_names.Last();
				if (UAZWidget* last_widget = GetUI<UAZWidget>(last_ui_name))
				{
					if (last_widget->bIsFocusable)
					{
						last_widget->SetFocus();
					}
				}
			}
		}
	}
}

void AAZHUD::CloseMsgBox_Top()
{
	if (msg_box_stack_.Num() > 0) {
		UAZWidget_MsgBoxBase* msg_box = msg_box_stack_.Top();

		if (nullptr == msg_box)
			return;

		if (true == msg_box->GetIsDisableBackBtnExit())
			return;

		CloseMsgBox(msg_box->GetName());
	}
}

void AAZHUD::RaiseOnTopFromStack(EUIName ui_name)
{
	if (scenes_stack.Contains(ui_name) == true)
	{
		scenes_stack.Remove(ui_name);
		scenes_stack.Emplace(ui_name);
	}
}

void AAZHUD::_OpenUI(UAZWidget* widget, FAZWidgetData* widget_data, bool is_immediately, bool is_pre_scene)
{
	if (widget_data->is_stay_in_viewport == false)
	{
		if (is_pre_scene == true)
		{
			int32 find_scene_index = 0;
			if (scenes_stack.Find(cur_scene_name_enum, find_scene_index) && 0 < find_scene_index)
			{
				find_scene_index -= 1;

				auto cur_scene_data = GetSceneData(scenes_stack[find_scene_index]);
				if (cur_scene_data == false)
				{
					ensureMsgf(false, TEXT("AAZHUD::_OpenUI cur_scene = nulpointer"));
					return;
				}

				if (cur_scene_data->child_widget_names.Contains((EUIName)widget_data->widget_id) == false)
				{
					cur_scene_data->child_widget_names.Add((EUIName)widget_data->widget_id);
				}

				widget->AddToViewport((int32)widget_data->layer);
				widget->OnOpen(is_immediately);
				widget->ForceHide();
				return;
			}
		}
		else
		{
			auto cur_scene_data = GetSceneData(cur_scene_name_enum);
			if (cur_scene_data == nullptr)
			{
				ensureMsgf(false, TEXT("AAZHUD::_OpenUI cur_scene = nullpointer"));
				return;
			}

			if (!cur_scene_data->child_widget_names.Contains((EUIName)widget_data->widget_id))
			{
				if (0 < cur_scene_data->child_widget_names.Num())
				{
					EUIName child_widget_enum_name = cur_scene_data->child_widget_names.Last();

					if (UAZWidget* pre_widget = GetUI<UAZWidget>(child_widget_enum_name))
					{
						pre_widget->OnCoveredUI();
					}
				}

				cur_scene_data->child_widget_names.Add((EUIName)widget_data->widget_id);
			}
		}
	}
	else
	{
		if (widget->IsInViewport())
		{
			return;
		}
	}

	if (widget->IsInViewport())
	{
		widget->RemoveFromParent();
		// If it is among the widgets, delete it.
		for (EUIName& scene_name : scenes_stack)
		{
			if (AZSceneData* scene_data = GetSceneData(scene_name))
			{
				const EUIName& ui_name = (EUIName)widget_data->widget_id;
				if (scene_data->child_widget_names.Contains(ui_name))
				{
					scene_data->child_widget_names.Remove(ui_name);
					break;
				}
			}
		}
	}

	widget->AddToViewport((int32)widget_data->layer);
	widget->OnOpen(is_immediately);

	if (hide_ui_flag > 0)
	{
		widget->ForceHide();
	}
	else
	{
		if (widget->GetIsForceHidden())
		{
			widget->RestoreFromForceHidden();
		}
	}

	if (widget_data->widget_id == (uint16)EUIName::AZWidget_Waiting)
	{
		waiting_widget_ = Cast<UAZWidget_Waiting>(widget);
	}

	UpdateWorldRender();
}

bool AAZHUD::CloseUI(EUIName ui_name, bool is_immediately, bool is_remove_stack, bool is_check_next_show_ui)
{
	UAZWidget* widget = GetUI<UAZWidget>(ui_name);

	if (widget == nullptr)
	{
		return false;
	}

	if (widget->GetOpenUIState() == EUIState::Closed)
	{
		return false;
	}

	widget->OnClose(is_immediately);

	if (is_remove_stack)
	{
		AZSceneData* cur_scene_data = GetSceneData(cur_scene_name_enum);
		if (cur_scene_data && cur_scene_data->child_widget_names.Contains(ui_name))
		{
			cur_scene_data->child_widget_names.Remove(ui_name);
		}
	}

	if (is_check_next_show_ui && OnShowUIListAfterCloseUI.Num() > 0)
	{
		OnShowUIListAfterCloseUI[0].ExecuteIfBound();
		OnShowUIListAfterCloseUI.RemoveAt(0);
	}

	UpdateWorldRender();

	return true;
}

bool AAZHUD::CloseUI(UAZWidget* widget, bool is_immediately, bool is_remove_stack, bool is_check_next_show_ui)
{
	return CloseUI(widget->GetWidgetNameEnum(), is_immediately, is_remove_stack, is_check_next_show_ui);
}

void AAZHUD::UpdateWorldRender()
{
	EUIName top_ui = GetTopWidgetName();
	FAZWidgetData* widget_data = GetWidgetData(top_ui);
	if (widget_data == nullptr)
	{
		return;
	}
	SetWorldRender(!widget_data->disable_world_rendering);
}

void AAZHUD::SetWorldRender(bool value)
{
	UGameViewportClient* game_viewport_client = GetWorld()->GetGameViewport();
	if (game_viewport_client == nullptr)
	{
		return;
	}
	game_viewport_client->bDisableWorldRendering = !value;
}

AAZHUD::AZSceneData* AAZHUD::GetSceneData(EUIName widget_name_enum)
{
	AZSceneData* scene_data = scene_datas.Find(widget_name_enum);
	if (scene_data == nullptr)
	{
		FAZWidgetData* widget_data = GetWidgetData(widget_name_enum);
		if (widget_data && widget_data->layer == EUILayer::Scene)
		{
			AZSceneData add_scene_data;
			add_scene_data.widget_name_enum = widget_name_enum;
			add_scene_data.child_widget_names.Empty();
			scene_datas.Add(widget_name_enum, add_scene_data);
			scene_data = scene_datas.Find(widget_name_enum);
		}
	}
	return scene_data;
}

EUIName AAZHUD::GetCurSceneNameEnum()
{
	return cur_scene_name_enum;
}

FAZWidgetData* AAZHUD::GetWidgetData(EUIName widget_name_enum)
{
	//얘가 문제인지 뭔지 아무튼 메모리 잘못읽음.
	if(game_instance_)
	{
		//맵전환할때 호출하면 못찾아는거 같음 확인바람
		if(game_instance_->hud_data_mgr)
		{
			return game_instance_->hud_data_mgr->GetWidgetData(widget_name_enum);
		}
	}
	return nullptr;
}

FAZWidgetData* AAZHUD::GetWidgetData(EUIName widget_name_enum) const
{
	return game_instance_->hud_data_mgr->GetWidgetData(widget_name_enum);
}

FAZWidgetData* AAZHUD::GetSubWidgetData(ESubUIName widget_name_enum)
{
	return game_instance_->hud_data_mgr->GetSubWidgetData(widget_name_enum);
}

EUIName AAZHUD::GetTopWidgetName()
{
	AZSceneData* cur_scene_data = GetSceneData(cur_scene_name_enum);

	if (!cur_scene_data)
	{
		return EUIName::None;
	}


	if (cur_scene_data->child_widget_names.Num() <= 0)
	{
		return EUIName::None;
	}

	return cur_scene_data->child_widget_names.Last();
}

bool AAZHUD::IsInViewportWaitingWidget()
{
	return waiting_widget_ ? waiting_widget_->IsInViewport() : false;
}

bool AAZHUD::IsCurScene(EUIName ui_name)
{
	return cur_scene_name_enum == ui_name ? true: false;
}

void AAZHUD::CloseScene(bool isBackButton)
{
	CloseScene(cur_scene_name_enum, true, isBackButton);
}

void AAZHUD::CloseScene(EUIName widget_name_enum, bool is_stack_delete, bool is_back_button)
{
	auto cur_scene_data = GetSceneData(widget_name_enum);
	if (cur_scene_data == nullptr)
	{
		return;
	}

	// FIXME Later implementation to call Exit popup when trying to pop when there is only one stack left
	if (1 >= scenes_stack.Num() && is_back_button == true)
	{
		//else
		//{
		//    // FIXME open popup
		//}
		return;
	}

	for (int32 i = cur_scene_data->child_widget_names.Num() - 1; i >= 0; --i)
	{
		FAZWidgetData* ui_widget_data = GetWidgetData(cur_scene_data->child_widget_names[i]);
		if (ui_widget_data && ui_widget_data->IsWidgetValid())
		{
			if (is_stack_delete == false)
			{
				ui_widget_data->GetWidget()->ForceHide();
			}
			else
			{
				// Close open popups when another scene is opened and closed
				CloseUI((EUIName)ui_widget_data->widget_id, true, false);
				cur_scene_data->child_widget_names.Pop();
			}
		}
		else
		{
			cur_scene_data->child_widget_names.Pop();
		}
	}

	if (is_stack_delete == true)
	{
		cur_scene_data->child_widget_names.Empty();
	}

	if (is_stack_delete == true)
	{
		if (0 < scenes_stack.Num())
		{
			scenes_stack.Pop();
			if (scenes_stack.Num() > 0)
			{
				// restore sequentially.
				OpenScene<UAZWidget>(scenes_stack.Top());
			}
		}
	}

	UpdateWorldRender();

	OnSceneClosed();
}

void AAZHUD::CloseAllUI()
{
	TMap<EUIName, FAZWidgetData> widget_datas;
	UAZHUDDataMgr* hud_data_mgr = game_instance_->hud_data_mgr;
	if (hud_data_mgr)
	{
		widget_datas = hud_data_mgr->GetWidgetDatas();

		for (auto& widget_pair : widget_datas)
		{
			FAZWidgetData& ui_widget_data = widget_pair.Value;

			if (ui_widget_data.IsWidgetValid())
			{
				UAZWidget* ui_widget = ui_widget_data.GetWidget();

				ui_widget->RemoveFromParent();
				ui_widget->MarkAsGarbage();
				ui_widget->ConditionalBeginDestroy();
				ui_widget = nullptr;
			}
		}
	}
	
	for (auto& kv : scene_datas)
	{
		kv.Value.child_widget_names.Empty();
	}

	scenes_stack.Empty();
	cur_scene_name_enum = EUIName::None;

	// FIXME Add message box code
}
