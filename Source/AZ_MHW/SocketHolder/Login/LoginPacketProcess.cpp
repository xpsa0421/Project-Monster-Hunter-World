#include "AZ_MHW/SocketHolder/PacketFunction.h"

#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"

// server(Login)
void UPacketFunction::LoginSigninRequest(UINT32 client_index, CS_LOGIN_SIGNIN_REQ* packet)
{
	auto P_user_id = game_instance_->ConvertCharToSqlTCHAR(packet->user_id);
	auto P_user_pw = game_instance_->ConvertCharToSqlTCHAR(packet->user_pw);
	UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	SC_LOGIN_SIGNIN_RES login_res_packet;
	login_res_packet.packet_id = (short)PACKET_ID::SC_LOGIN_SIGNIN_RES;
	login_res_packet.packet_length = sizeof(login_res_packet);
	login_res_packet.client_index = client_index;
	login_res_packet.success = 0;
	FString id(packet->user_id);
	game_instance_->game_cache_info_->LoginRequest(client_index, id);
	/*if (game_instance_->odbc->LoginCheckSQL(P_user_id, P_user_pw))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);
		login_res_packet.success = 1;
	}*/
	game_instance_->SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
}

void UPacketFunction::LoginSignupRequest(UINT32 client_index, CS_LOGIN_SIGNUP_REQ* packet)
{
	dbitem record;

	auto P_user_id = game_instance_->ConvertCharToSqlTCHAR(packet->user_id); ;
	auto P_user_pw = game_instance_->ConvertCharToSqlTCHAR(packet->user_pw); ;

	UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	record.name = P_user_id;
	record.pass = P_user_pw;

	SC_LOGIN_SIGNUP_RES login_res_packet;
	login_res_packet.packet_id = (short)PACKET_ID::SC_LOGIN_SIGNUP_RES;
	login_res_packet.packet_length = sizeof(login_res_packet);
	/*if (game_instance_->Fclient_connect_.IsBound() == true)	game_instance_->Fclient_connect_.Execute();
	if (game_instance_->odbc->AddSQL(record))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);
		login_res_packet.success = 0;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);
		login_res_packet.success = 1;
	}*/

	FString id(packet->user_id);
	if (game_instance_->game_cache_info_->SignupRequest(id) == false)
	{
		login_res_packet.success = 1;
	}
	game_instance_->SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
}

// client(Login)
void UPacketFunction::LoginSigninResponse(SC_LOGIN_SIGNIN_RES* packet)
{
	if (packet->success != 0)
	{
		//팝업 띄우기
		game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("로그인에 실패하셨습니다."), EUIMsgBoxBtnType::Confirm,
			game_instance_->login_mgr, TEXT(""), L"", L"", L"확인");
	}
	else
	{
		game_instance_->game_cache_info_->LoginResponse(packet->client_index);
		game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::AuthGameServer);
	}
}

void UPacketFunction::LoginSignupResponse(SC_LOGIN_SIGNUP_RES* packet)
{
	if (packet->success != 0)
	{
		//팝업 띄우기
		game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("회원가입에 실패하였습니다."), EUIMsgBoxBtnType::Confirm,
			game_instance_->login_mgr, TEXT(""), L"", L"", L"확인");
	}
	else
	{
		game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("회원가입에 성공했습니다."), EUIMsgBoxBtnType::Confirm,
			game_instance_->login_mgr, TEXT(""), L"", L"", L"확인");
	}
}