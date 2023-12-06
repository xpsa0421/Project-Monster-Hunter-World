// Fill out your copyright notice in the Description page of Project Settings.


#include "Chatting.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
//#include "Client_To_Server.h"
#include "AZGameInstance.h"


void UChatting::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("[NativeConstruct]\n"));

	ChatHistory = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatHistory")));
	ChatMsgBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

	az_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	if (az_game_instance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UChatting if]\n"));
		//az_game_instance->Fuc_boradcast_success.BindUFunction(this, FName("ChatHistory_Msg"));
	}
}

UChatting::UChatting(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("[UChatting]\n"));
}

void UChatting::ChatMsgSend(FString msg)
{
	/*LOGIN_REQUEST_PACKET login_send_packet;
	login_send_packet.packet_id = (int)PACKET_ID::CHAT_SEND_REQUEST;
	strcpy_s(login_send_packet.user_id, sizeof(login_send_packet.user_id), FStringToCharArray(*msg));
	login_send_packet.packet_length = sizeof(login_send_packet);

	UE_LOG(LogTemp, Warning, TEXT("[ChatMsgSend.cpp] pakcet_id : %d, pakcet_userid : %s \n"), login_send_packet.packet_id, *CharArrayToFString(login_send_packet.user_id));

	az_game_instance->Server_Packet_Send((char*)&login_send_packet, login_send_packet.packet_length);*/
}


// Fstring to char
char* UChatting::FStringToCharArray(FString fString)
{
	std::string stdString(TCHAR_TO_UTF8(*fString));
	char* charArray = new char[stdString.length() + 1];
	strcpy_s(charArray, sizeof(charArray), stdString.c_str());

	return charArray;
}

// char To FString
FString UChatting::CharArrayToFString(const char* CharArray)
{
	std::string StdString = CharArray;

	return FString(StdString.c_str());
}

void UChatting::ChatHistory_Msg(FString msg)
{
	UE_LOG(LogTemp, Warning, TEXT("ChatHistory_Msg / %s"), *msg)

	// TODO 채팅 브로드캐스트 델리게이트 받고 채팅창에 글씨 넣는건데 ... 코드로 적용하면안됨..
	//AddChatMessage(msg);
}

void UChatting::AddChatMessage(const FString& Message)
{
	// Text 오브젝트를 생성하고, ScrollBox에 추가한다.
	UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatHistory);
	NewTextBlock->SetText(FText::FromString(Message));

	ChatHistory->AddChild(NewTextBlock);
	ChatHistory->ScrollToEnd(); // 가장 최근 채팅을 보기 위해, 스크롤을 가장 아래로 내린다.
}
