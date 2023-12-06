// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "Components/EditableTextBox.h"
#include "AZWidget_CharacterCreate.generated.h"

class AAZPlayer_Mannequin;
class AAZPlayerState_Client;
/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_CharacterCreate : public UAZWidget
{
	GENERATED_BODY()
	
private:
	FCharacterSimpleInfo choose_character_info_;
	
	UPROPERTY() AAZPlayerState_Client* current_selected_character_state_;
	UPROPERTY() AAZPlayer_Mannequin* current_selected_character_;
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void Update() override;

public:
	UFUNCTION() void OnConfirm();
	UFUNCTION() void OnBack();

	UFUNCTION() void OnBlack();
	UFUNCTION() void OnGray();
	UFUNCTION() void OnRed();
	UFUNCTION() void OnBrown();
	UFUNCTION() void OnWhite();

	UFUNCTION() void OnHair0();
	UFUNCTION() void OnHair1();
	UFUNCTION() void OnHair2();
	UFUNCTION() void OnHair3();
	
public:
	UPROPERTY(meta = (BindWidget)) class UAZButton* confirm_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* back_;

	UPROPERTY(meta = (BindWidget)) class UAZButton* black_button_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* gray_button_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* red_button_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* brown_button_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* white_button_;


	UPROPERTY(meta = (BindWidget)) class UAZButton* hair_button_0_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* hair_button_1_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* hair_button_2_;
	UPROPERTY(meta = (BindWidget)) class UAZButton* hair_button_3_;

	UPROPERTY(meta = (BindWidget)) UEditableTextBox* nick_;
};
