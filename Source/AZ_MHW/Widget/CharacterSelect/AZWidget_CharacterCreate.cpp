// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterCreate.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Player/AZPlayer_Mannequin.h"
#include "PlayerState/AZPlayerState_Client.h"

void UAZWidget_CharacterCreate::Init()
{
	Super::Init();

	AZ_PRINT_LOG_IF_FALSE(confirm_);
	confirm_->OnClicked.AddDynamic(this,&UAZWidget_CharacterCreate::OnConfirm);
	AZ_PRINT_LOG_IF_FALSE(back_);
	back_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnBack);
	AZ_PRINT_LOG_IF_FALSE(black_button_);
	black_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnBlack);
	AZ_PRINT_LOG_IF_FALSE(gray_button_);
	gray_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnGray);
	AZ_PRINT_LOG_IF_FALSE(red_button_);
	red_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnRed);
	AZ_PRINT_LOG_IF_FALSE(brown_button_);
	brown_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnBrown);
	AZ_PRINT_LOG_IF_FALSE(white_button_);
	white_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnWhite);


	AZ_PRINT_LOG_IF_FALSE(hair_button_0_);
	hair_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair0);
	AZ_PRINT_LOG_IF_FALSE(hair_button_1_);
	hair_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair1);
	AZ_PRINT_LOG_IF_FALSE(hair_button_2_);
	hair_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair2);
	AZ_PRINT_LOG_IF_FALSE(hair_button_3_);
	hair_button_3_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair3);
}

void UAZWidget_CharacterCreate::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	
	//보여주기용도로 객체 생성
	current_selected_character_state_ = game_instance_->GetPlayerController()->GetPlayerState<AAZPlayerState_Client>();
	current_selected_character_ = GetWorld()->SpawnActor<AAZPlayer_Mannequin>();
	current_selected_character_->GetRootComponent()->SetWorldLocation(FVector(0,0,0));
	current_selected_character_->SetPlayerState(current_selected_character_state_);
}

void UAZWidget_CharacterCreate::Update()
{
	
}

void UAZWidget_CharacterCreate::OnConfirm()
{
	FString id = nick_->GetText().ToString();
	FCStringAnsi::Strncpy(choose_character_info_.character_nick_, TCHAR_TO_ANSI(*id), sizeof(choose_character_info_.character_nick_) - 1);
	current_selected_character_state_->nickname_ = id;
	choose_character_info_.arm_id_ = current_selected_character_state_->equipment_state_.arm_item_id;
	choose_character_info_.body_id_ = current_selected_character_state_->equipment_state_.body_item_id;
	choose_character_info_.leg_id_ = current_selected_character_state_->equipment_state_.leg_item_id;
	choose_character_info_.waist_id_ = current_selected_character_state_->equipment_state_.waist_item_id;
	choose_character_info_.head_id_ = current_selected_character_state_->equipment_state_.head_item_id;
	choose_character_info_.hair_color_ = current_selected_character_state_->equipment_state_.hair_color;
	choose_character_info_.hair_id_ = current_selected_character_state_->equipment_state_.hair_item_id;
	choose_character_info_.weapon_id = current_selected_character_state_->equipment_state_.first_weapon_item_id;
	choose_character_info_.weapon_type = current_selected_character_state_->equipment_state_.weapon_type;

	CS_PLAYER_CHARACTER_CREATE_REQ packet;
	packet.create_info = choose_character_info_;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
}

void UAZWidget_CharacterCreate::OnBack()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_CharacterSelect"));
}





void UAZWidget_CharacterCreate::OnBlack()
{
	current_selected_character_state_->equipment_state_.hair_color=FVector4f(0,0,0,1);
	current_selected_character_->SetHairColor();
	choose_character_info_.hair_color_ =  current_selected_character_state_->equipment_state_.hair_color;
}

void UAZWidget_CharacterCreate::OnGray()
{
	current_selected_character_state_->equipment_state_.hair_color= FVector4f(0.2,0.05,0,1);
	current_selected_character_->SetHairColor();
	choose_character_info_.hair_color_ =  current_selected_character_state_->equipment_state_.hair_color;
}

void UAZWidget_CharacterCreate::OnRed()
{
	current_selected_character_state_->equipment_state_.hair_color = FVector4f(0.8,0,0,1);
	current_selected_character_->SetHairColor();
	choose_character_info_.hair_color_ =  current_selected_character_state_->equipment_state_.hair_color;
}

void UAZWidget_CharacterCreate::OnBrown()
{
	current_selected_character_state_->equipment_state_.hair_color =  FVector4f(0.5,0.15,0,1);
	current_selected_character_->SetHairColor();
	choose_character_info_.hair_color_ =  current_selected_character_state_->equipment_state_.hair_color;
}

void UAZWidget_CharacterCreate::OnWhite()
{
	current_selected_character_state_->equipment_state_.hair_color =  FVector4f(1,1,1,1);
	current_selected_character_->SetHairColor();
	choose_character_info_.hair_color_ =  current_selected_character_state_->equipment_state_.hair_color;
}


void UAZWidget_CharacterCreate::OnHair0()
{
	current_selected_character_->ChangeEquipment(12501);
	choose_character_info_.hair_id_ =current_selected_character_state_->equipment_state_.hair_item_id;
}

void UAZWidget_CharacterCreate::OnHair1()
{
	current_selected_character_->ChangeEquipment(12502);
	choose_character_info_.hair_id_ =current_selected_character_state_->equipment_state_.hair_item_id;
}

void UAZWidget_CharacterCreate::OnHair2()
{
	current_selected_character_->ChangeEquipment(12503);
	choose_character_info_.hair_id_ =current_selected_character_state_->equipment_state_.hair_item_id;
}

void UAZWidget_CharacterCreate::OnHair3()
{
	int x = FMath::RandRange(12501,12532);
	current_selected_character_->ChangeEquipment(x);
	choose_character_info_.hair_id_ =current_selected_character_state_->equipment_state_.hair_item_id;
}
