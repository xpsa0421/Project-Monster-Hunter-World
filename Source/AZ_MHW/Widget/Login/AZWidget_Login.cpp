// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Login/AZWidget_Login.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "AZ_MHW/Widget/AZWidget_Waiting.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "Misc/CString.h"

//#include "AZ_MHW/CommonSource/Define/"

void UAZWidget_Login::Init()
{
	Super::Init();

	AZ_PRINT_LOG_IF_FALSE(c_btn_login_);
	c_btn_login_->OnClicked.RemoveDynamic(this, &UAZWidget_Login::OnClicked_Login);
	c_btn_login_->OnClicked.AddDynamic(this, &UAZWidget_Login::OnClicked_Login);

	AZ_PRINT_LOG_IF_FALSE(c_btn_any_press_);
	c_btn_any_press_->OnClicked.RemoveDynamic(this, &UAZWidget_Login::OnTouchAnyPress);
	c_btn_any_press_->OnClicked.AddDynamic(this, &UAZWidget_Login::OnTouchAnyPress);

	AZ_PRINT_LOG_IF_FALSE(c_btn_close_);
	c_btn_close_->OnClicked.RemoveDynamic(this, &UAZWidget_Login::OnClicked_Close);
	c_btn_close_->OnClicked.AddDynamic(this, &UAZWidget_Login::OnClicked_Close);

	AZ_PRINT_LOG_IF_FALSE(c_btn_sign_up_);
	c_btn_sign_up_->OnClicked.RemoveDynamic(this, &UAZWidget_Login::OnClicked_SignUp);
	c_btn_sign_up_->OnClicked.AddDynamic(this, &UAZWidget_Login::OnClicked_SignUp);

	panel_state_[(int32)EPanelState::TouchScreen] = GetOwnWidget<UWidget>(FString(TEXT("panel_state_touch")));
	panel_state_[(int32)EPanelState::IDPasswordLogin] = GetOwnWidget<UWidget>(FString(TEXT("panel_state_id")));

	for (int32 i = 0; i < (int32)EPanelState::Max; ++i)
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[i]);
		panel_state_[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAZWidget_Login::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);

	game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::WaitingForTouch);
}

void UAZWidget_Login::SetLoginMode(ELogInMode login_mode)
{
	for (int32 i = 0; i < (int32)EPanelState::Max; ++i)
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[i]);
		panel_state_[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	switch (login_mode)
	{
	case ELogInMode::TouchConnect:
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[(int32)EPanelState::TouchScreen]);
		panel_state_[(int32)EPanelState::TouchScreen]->SetVisibility(ESlateVisibility::Visible);
		c_btn_any_press_->SetVisibility(ESlateVisibility::Visible);

	}break;
	case ELogInMode::IDPassword:
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[(int32)EPanelState::IDPasswordLogin]);
		panel_state_[(int32)EPanelState::IDPasswordLogin]->SetVisibility(ESlateVisibility::Visible);
	}break;
	}
}

void UAZWidget_Login::OnTouchAnyPress()
{
	UE_LOG(LogTemp, Warning, TEXT("client open\n"));
	c_btn_any_press_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UAZLoginMgr* login_mgr = game_instance_->login_mgr;
	if (login_mgr != nullptr)
	{
		login_mgr->ChangeSequence(UAZLoginMgr::ESequence::ConnectGameServer);
	}
}

void UAZWidget_Login::OnClicked_Login()
{
	game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::AuthGameServer);

	FString id = c_id_->GetText().ToString();
	FString password = c_pass_->GetText().ToString();

	UE_LOG(LogTemp, Warning, TEXT("로그인 체크 id : %s / pw :%s\n"), *c_id_->GetText().ToString(), *c_pass_->GetText().ToString());

	CS_LOGIN_SIGNIN_REQ login_send_packet;
	login_send_packet.packet_id = (unsigned short)PACKET_ID::CS_LOGIN_SIGNIN_REQ;
	FCStringAnsi::Strncpy(login_send_packet.user_id, TCHAR_TO_ANSI(*id), sizeof(login_send_packet.user_id) - 1);
	FCStringAnsi::Strncpy(login_send_packet.user_pw, TCHAR_TO_ANSI(*password), sizeof(login_send_packet.user_pw) - 1);
	login_send_packet.packet_length = sizeof(login_send_packet);

	game_instance_->game_cache_info_->SetClientId(id);
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&login_send_packet, login_send_packet.packet_length);
}

void UAZWidget_Login::OnClicked_Close()
{
	game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::GameExit);
}

void UAZWidget_Login::OnClicked_SignUp()
{
	FString id = c_id_->GetText().ToString();
	FString password = c_pass_->GetText().ToString();

	CS_LOGIN_SIGNUP_REQ login_send_packet;
	login_send_packet.packet_id = (unsigned short)PACKET_ID::CS_LOGIN_SIGNUP_REQ;
	FCStringAnsi::Strncpy(login_send_packet.user_id, TCHAR_TO_ANSI(*id), sizeof(login_send_packet.user_id) - 1);
	FCStringAnsi::Strncpy(login_send_packet.user_pw, TCHAR_TO_ANSI(*password), sizeof(login_send_packet.user_pw) - 1);
	login_send_packet.packet_length = sizeof(login_send_packet);
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&login_send_packet, login_send_packet.packet_length);
}
