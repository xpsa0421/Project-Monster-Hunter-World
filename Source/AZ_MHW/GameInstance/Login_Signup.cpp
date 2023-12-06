// Fill out your copyright notice in the Description page of Project Settings.


#include "Login_Signup.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/WidgetLayoutLibrary.h"
#include "AZGameInstance.h"
//#include "Client_To_Server.h"

void ULogin_Signup::NativeConstruct()
{
	Super::NativeConstruct();

	az_game_instance = Cast<UAZGameInstance>(GetGameInstance());
}

bool ULogin_Signup::Login_Signup(FString id, FString pw, FString pw2)
{
	signup_id = id;
	signup_pw = pw;
	signup_check_pw = pw2;	

	// TODO : 아이디 중복체크 기능 추가 (true 대신 db에서 id 있는지 확인)
	if (true)
	{
		// 패스워드 동일한지 체크
		if (signup_pw.Equals(signup_check_pw, ESearchCase::CaseSensitive))
		{
			UE_LOG(LogTemp, Warning, TEXT("Password Success"));
			// TODO1 DB조회 대신 id,pw Send로 변경 
			//az_game_instance->SignupRecord(signup_id, signup_pw);
			/*Signup_Packet_Send(signup_id, signup_pw, PACKET_ID::SIGNIN_REQUEST); */

			return true;
		}
		else
		{
			::MessageBox(NULL, L"Passwords are not the same", L"Signup", 0);
			UE_LOG(LogTemp, Warning, TEXT("Password Fail"));

			return false;
		}
	}
	//return true;
}

void ULogin_Signup::Signup_Packet_Send(FString id, FString pw, PACKET_ID packet_id)
{
	/*LOGIN_REQUEST_PACKET login_send_packet;
	login_send_packet.packet_id = (int)packet_id;
	strcpy_s(login_send_packet.user_id, sizeof(login_send_packet.user_id), TCHAR_TO_ANSI(*id));
	strcpy_s(login_send_packet.user_pw, sizeof(login_send_packet.user_pw), TCHAR_TO_ANSI(*pw));
	login_send_packet.packet_length = sizeof(login_send_packet);

	UE_LOG(LogTemp, Warning, TEXT("[Signup_Packet_Send] pakcet_id : %d, pakcet_userid : %s\n"), login_send_packet.packet_id, login_send_packet.user_id);

	az_game_instance->Server_Packet_Send((char*)&login_send_packet, login_send_packet.packet_length);*/
}