// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/MsgBox/AZWidget_MsgBoxBasic.h"

#include "AZ_MHW/HUD/AZHUD.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UAZWidget_MsgBoxBasic::Init()
{
	Super::Init();

	if (button_.Num() <= 0)
	{
		button_.Emplace(c_btn_left_);
		button_.Emplace(c_btn_right_);
		button_.Emplace(c_btn_cancle_);
		button_.Emplace(c_btn_close_);
		button_.Emplace(c_btn_fill_screen_);
	}
	if (button_text_.Num() <= 0)
	{
		button_text_.Emplace(c_text_left_btn_);
		button_text_.Emplace(c_text_right_btn_);
	}

	if (button_[0])
		button_[0]->OnClicked.AddDynamic(this, &UAZWidget_MsgBoxBasic::OnTapLeft);
	if (button_[1])
		button_[1]->OnClicked.AddDynamic(this, &UAZWidget_MsgBoxBasic::OnTapRight);
	if (button_[2])
		button_[2]->OnClicked.AddDynamic(this, &UAZWidget_MsgBoxBasic::OnTapCancle);
	if (button_[3])
		button_[3]->OnClicked.AddDynamic(this, &UAZWidget_MsgBoxBasic::OnTapClose);
	if (button_[4])
		button_[4]->OnClicked.AddDynamic(this, &UAZWidget_MsgBoxBasic::OnTapLeft);

	c_ani_ = GetWidgetAnimation(TEXT("c_ani"));
}

void UAZWidget_MsgBoxBasic::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
}

void UAZWidget_MsgBoxBasic::Destroy()
{
	if (handler_left_.IsBound())
		handler_left_.Unbind();

	if (handler_right_.IsBound())
		handler_right_.Unbind();

	if (handler_cancle_.IsBound())
		handler_cancle_.Unbind();

	if (handler_close_.IsBound())
		handler_close_.Unbind();

	StopAnimToOrigin(c_ani_);
	Super::Destroy();
}

void UAZWidget_MsgBoxBasic::SetButtonType(EUIMsgBoxBtnType button_type)
{
	button_type_ = button_type;
	switch (button_type_)
	{
	case EUIMsgBoxBtnType::OkOrCancel:
		SetButtonText(EMsgEventButtonType::Left, "OkOrCancel");
		ShowButton(EMsgEventButtonType::Left, true);
		ShowButton(EMsgEventButtonType::Right, false);
		ShowButton(EMsgEventButtonType::Cancle, true);
		ShowButton(EMsgEventButtonType::Close, true);
		ShowButton(EMsgEventButtonType::Screen, false);
		if (c_full_screen_box_)
			c_full_screen_box_->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EUIMsgBoxBtnType::Confirm:
		SetButtonText(EMsgEventButtonType::Left, "Confirm");
		ShowButton(EMsgEventButtonType::Left, true);
		ShowButton(EMsgEventButtonType::Right, false);
		ShowButton(EMsgEventButtonType::Cancle, false);
		ShowButton(EMsgEventButtonType::Close, false);
		ShowButton(EMsgEventButtonType::Screen, false);
		if (c_full_screen_box_)
			c_full_screen_box_->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EUIMsgBoxBtnType::LeftOrRight:
		SetButtonText(EMsgEventButtonType::Left, "");
		SetButtonText(EMsgEventButtonType::Right, "");
		ShowButton(EMsgEventButtonType::Left, true);
		ShowButton(EMsgEventButtonType::Right, true);
		ShowButton(EMsgEventButtonType::Cancle, false);
		ShowButton(EMsgEventButtonType::Close, true);
		ShowButton(EMsgEventButtonType::Screen, false);
		if (c_full_screen_box_)
			c_full_screen_box_->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EUIMsgBoxBtnType::ScreenButton:
		ShowButton(EMsgEventButtonType::Left, false);
		ShowButton(EMsgEventButtonType::Right, false);
		ShowButton(EMsgEventButtonType::Cancle, false);
		ShowButton(EMsgEventButtonType::Close, false);
		ShowButton(EMsgEventButtonType::Screen, true);
		if (c_full_screen_box_)
			c_full_screen_box_->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(c_ani_, 0, 0);
		break;
	}
}

void UAZWidget_MsgBoxBasic::SetButtonText(EMsgEventButtonType button_type, FString text)
{
	if (button_text_[(int32)button_type] == nullptr)
		return;

	button_text_[(int32)button_type]->SetText(FText::FromString(text));
}

void UAZWidget_MsgBoxBasic::ShowButton(EMsgEventButtonType button_type, bool is_show)
{
	if (nullptr == button_[(int32)button_type])
		return;

	if (is_show == true)
		button_[(int32)button_type]->SetVisibility(ESlateVisibility::Visible);
	else
		button_[(int32)button_type]->SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_MsgBoxBasic::OnTapClose_FromOutside()
{
	OnTapClose();
}

FReply UAZWidget_MsgBoxBasic::NativeOnKeyDown(const FGeometry& in_geometry, const FKeyEvent& in_key_event)
{
	if (msg_box_type_ != EUIMsgBoxType::Basic)
		return Super::NativeOnKeyDown(in_geometry, in_key_event);

	if (button_type_ != EUIMsgBoxBtnType::Confirm)
		return Super::NativeOnKeyDown(in_geometry, in_key_event);

#if PLATFORM_WINDOWS
	if (in_key_event.GetKey() == EKeys::Escape)
	{
		OnTapLeft();
	}
#endif // 

	return Super::NativeOnKeyDown(in_geometry, in_key_event);
}

FReply UAZWidget_MsgBoxBasic::NativeOnMouseButtonDown(const FGeometry& in_geometry, const FPointerEvent& in_mouse_event)
{
	if (msg_box_type_ != EUIMsgBoxType::Basic)
		return Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

	if (button_type_ != EUIMsgBoxBtnType::Confirm)
		return Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

#if WITH_EDITOR
	if (in_mouse_event.GetEffectingButton() == EKeys::ThumbMouseButton)
	{
		OnTapLeft();
	}
#endif // 

	return Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);
}

void UAZWidget_MsgBoxBasic::OnTapLeft()
{
	handler_left_.ExecuteIfBound();

	CloseMsgBox();
}

void UAZWidget_MsgBoxBasic::OnTapRight()
{
	handler_right_.ExecuteIfBound();

	CloseMsgBox();
}

void UAZWidget_MsgBoxBasic::OnTapCancle()
{
	if (button_type_ == EUIMsgBoxBtnType::OkOrCancel)
		handler_cancle_.ExecuteIfBound();

	CloseMsgBox();
}

void UAZWidget_MsgBoxBasic::OnTapClose()
{
	OnTapCancle();
}

void UAZWidget_MsgBoxBasic::CloseMsgBox()
{
	if (handler_left_.IsBound())
		handler_left_.Unbind();

	if (handler_right_.IsBound())
		handler_right_.Unbind();

	if (handler_cancle_.IsBound())
		handler_cancle_.Unbind();

	if (handler_close_.IsBound())
		handler_close_.Unbind();

	AAZHUD* hud = GetHUD();
	if (hud)
		hud->CloseMsgBox(GetName());
}
