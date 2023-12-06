// Fill out your copyright notice in the Description page of Project Settings.


#include "Login.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/WidgetLayoutLibrary.h"
#include "AZGameInstance.h"

void ULogin::NativeConstruct()
{
	Super::NativeConstruct();

	az_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	if (az_game_instance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULogin NativeConstruct]\n"));
		//az_game_instance->Fuc_in_game_connect.BindUFunction(this, FName("InGameConnect"));
	}
}

FString ULogin::SetLoginInfo(FString id, FString pw)
{
	/*LOGIN_REQUEST_PACKET login_send_packet;
	login_send_packet.packet_id = (int)PACKET_ID::LOGIN_REQUEST;
	strcpy_s(login_send_packet.user_id, sizeof(login_send_packet.user_id), TCHAR_TO_ANSI(*id));
	strcpy_s(login_send_packet.user_pw, sizeof(login_send_packet.user_pw), TCHAR_TO_ANSI(*pw));
	login_send_packet.packet_length = sizeof(login_send_packet);

	login_id = id;
	login_pw = pw;

	UE_LOG(LogTemp, Warning, TEXT("[Login.cpp] pakcet_id : %d, pakcet_userid : %s\n"), login_send_packet.packet_id, login_send_packet.user_id);

	az_game_instance->Server_Packet_Send((char*)&login_send_packet, login_send_packet.packet_length);*/

	return login_id;
}

void ULogin::ServerConnect()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerConnect\n"), );

	az_game_instance->Server_Connect();
}

void ULogin::InGameConnect()
{
	UE_LOG(LogTemp, Warning, TEXT("InGameConnect\n"));

	login_to_ingame = true;

	az_game_instance->InGameAccept();
}