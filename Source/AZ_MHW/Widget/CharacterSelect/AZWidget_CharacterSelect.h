// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_CharacterSelect.generated.h"

class AAZPlayer_Mannequin;
class AAZPlayerState_Client;
/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_CharacterSelect : public UAZWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void Update() override;

public:
	UPROPERTY(meta = (BindWidget)) class UWidget* new_game_0_;
	UPROPERTY(meta = (BIndWidget)) class UWidget* new_game_1_;
	UPROPERTY(meta = (BindWidget)) class UWidget* new_game_2_;
	UPROPERTY(meta = (BindWidget)) class UWidget* character_slot_0_;
	UPROPERTY(meta = (BindWidget)) class UWidget* character_slot_1_;
	UPROPERTY(meta = (BindWidget)) class UWidget* character_slot_2_;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* character_nick_name_0_;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* character_nick_name_1_;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* character_nick_name_2_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* select_button_0_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* select_button_1_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* select_button_2_;

public:
	UFUNCTION() void OnNewGame();
	UFUNCTION() void OnSelectCharacter_0();
	UFUNCTION() void OnSelectCharacter_1();
	UFUNCTION() void OnSelectCharacter_2();
	UFUNCTION() void OnHoverCharacter_0();
	UFUNCTION() void OnHoverCharacter_1();
	UFUNCTION() void OnHoverCharacter_2();


public:
	
	UPROPERTY()
	AAZPlayerState_Client* current_selected_character_state_;
	UPROPERTY()
	AAZPlayer_Mannequin* current_selected_character_;
};
