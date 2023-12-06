// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Chatting.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UChatting : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	UChatting(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, Category = Chatting)
	void ChatMsgSend(FString msg);
	char* FStringToCharArray(FString fstring);
	FString CharArrayToFString(const char* CharArray);

public: //ScorllBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ChatHistory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextBlock* ChatMsgBlock;

public:
	UFUNCTION()
	void ChatHistory_Msg(FString msg);

	UFUNCTION(BlueprintCallable, Category = Chatting)
	void AddChatMessage(const FString& Message);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAZGameInstance* az_game_instance;
};