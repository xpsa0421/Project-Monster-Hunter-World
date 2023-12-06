// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterSelect.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "Character/Player/AZPlayer_Mannequin.h"
#include "Components/TextBlock.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/AZPlayerState_Client.h"

void UAZWidget_CharacterSelect::Init()
{
	Super::Init();
	AZ_PRINT_LOG_IF_FALSE(new_game_0_);
	new_game_0_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(new_game_1_);
	new_game_1_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(new_game_2_);
	new_game_2_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(character_slot_0_);
	character_slot_0_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(character_slot_1_);
	character_slot_1_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(character_slot_2_);
	character_slot_2_->SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_CharacterSelect::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ packet;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
	
	//보여주기용도로 객체 생성하고 
	current_selected_character_state_ = game_instance_->GetPlayerController()->GetPlayerState<AAZPlayerState_Client>();
	current_selected_character_ = GetWorld()->SpawnActor<AAZPlayer_Mannequin>();
	current_selected_character_->GetRootComponent()->SetWorldLocation(FVector(0,0,0));
	current_selected_character_->spring_arm_comp_->SetWorldLocation(FVector(0,75,-5000));
}

void UAZWidget_CharacterSelect::Update()
{
	auto character_array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	if (character_array.Num() == 0)
	{
		new_game_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		new_game_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		new_game_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
	}
	if (character_array.Num() == 1)
	{
		character_slot_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_0_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[0].character_nick_)));
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_0);
		select_button_0_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_0);

		new_game_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		new_game_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
	}
	if (character_array.Num() == 2)
	{
		character_slot_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_0_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[0].character_nick_)));
		character_slot_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_1_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[1].character_nick_)));
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_0);
		select_button_0_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_0);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_1);
		select_button_1_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_1);

		new_game_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
	}
	if (character_array.Num() == 3)
	{
		character_slot_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_0_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[0].character_nick_)));
		character_slot_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_1_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[1].character_nick_)));
		character_slot_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_2_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[2].character_nick_)));
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_0);
		select_button_0_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_0);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_1);
		select_button_1_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_1);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_2);
		select_button_2_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_2);
	}

}

void UAZWidget_CharacterSelect::OnNewGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_CharacterCreate"));
}

void UAZWidget_CharacterSelect::OnSelectCharacter_0()
{
	CREATE_PLAYER_CHARACTER_PACKET packet;
	packet.character_index = current_selected_character_state_->cid_;
	packet.packet_id = (int)PACKET_ID::CS_PLAYER_ORIGIN_CREATE_REQ;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
	//UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/ServerDebug/ServerDebug_Level"),true,"?game=/Game/AZ/GameMode/BP_InGame.BP_InGame_C");
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_Main"),true,"?game=/Game/AZ/GameMode/BP_InGame.BP_InGame_C");
}

void UAZWidget_CharacterSelect::OnSelectCharacter_1()
{
	CREATE_PLAYER_CHARACTER_PACKET packet;
	packet.character_index = current_selected_character_state_->cid_;
	packet.packet_id = (int)PACKET_ID::CS_PLAYER_ORIGIN_CREATE_REQ;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
	//UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/ServerDebug/ServerDebug_Level"),true,"?game=/Game/AZ/GameMode/BP_InGame.BP_InGame_C");
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_Main"),true,"?game=/Game/AZ/GameMode/BP_InGame.BP_InGame_C");
}

void UAZWidget_CharacterSelect::OnSelectCharacter_2()
{
	CREATE_PLAYER_CHARACTER_PACKET packet;
	packet.character_index = current_selected_character_state_->cid_;
	packet.packet_id = (int)PACKET_ID::CS_PLAYER_ORIGIN_CREATE_REQ;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
	//UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/ServerDebug/ServerDebug_Level"),true,"?game=/Game/AZ/GameMode/BP_InGame.BP_InGame_C");
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_Main"),true,"?game=/Game/AZ/GameMode/BP_InGame.BP_InGame_C");
}

void UAZWidget_CharacterSelect::OnHoverCharacter_0()
{
	auto array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	game_instance_->game_cache_info_->current_character_index_ = array[0].character_index_;
	current_selected_character_state_->nickname_ = array[0].character_nick_;
	current_selected_character_state_->cid_= array[0].character_index_; 
	current_selected_character_state_->equipment_state_.arm_item_id = array[0].arm_id_;
	current_selected_character_state_->equipment_state_.body_item_id =array[0].body_id_;
	current_selected_character_state_->equipment_state_.leg_item_id =array[0].leg_id_;
	current_selected_character_state_->equipment_state_.waist_item_id =array[0].waist_id_;
	current_selected_character_state_->equipment_state_.hair_item_id =array[0].hair_id_;
	current_selected_character_state_->equipment_state_.hair_color =array[0].hair_color_;
	current_selected_character_state_->equipment_state_.weapon_type =array[0].weapon_type;
	current_selected_character_state_->equipment_state_.first_weapon_item_id =array[0].weapon_id;
	current_selected_character_->SetPlayerState(current_selected_character_state_);
	
	current_selected_character_->spring_arm_comp_->SetWorldLocation(FVector(0,75,75));
}

void UAZWidget_CharacterSelect::OnHoverCharacter_1()
{
	auto array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	game_instance_->game_cache_info_->current_character_index_ = array[1].character_index_;
	current_selected_character_state_->nickname_ = array[1].character_nick_;
	current_selected_character_state_->cid_= array[1].character_index_; 
	current_selected_character_state_->equipment_state_.arm_item_id = array[1].arm_id_;
	current_selected_character_state_->equipment_state_.body_item_id =array[1].body_id_;
	current_selected_character_state_->equipment_state_.leg_item_id =array[1].leg_id_;
	current_selected_character_state_->equipment_state_.waist_item_id =array[1].waist_id_;
	current_selected_character_state_->equipment_state_.hair_item_id =array[1].hair_id_;
	current_selected_character_state_->equipment_state_.hair_color =array[1].hair_color_;
	current_selected_character_state_->equipment_state_.weapon_type =array[1].weapon_type;
	current_selected_character_state_->equipment_state_.first_weapon_item_id =array[1].weapon_id;
	current_selected_character_->SetPlayerState(current_selected_character_state_);
	current_selected_character_->spring_arm_comp_->SetWorldLocation(FVector(0,75,75));
}

void UAZWidget_CharacterSelect::OnHoverCharacter_2()
{
	auto array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	game_instance_->game_cache_info_->current_character_index_ = array[2].character_index_;
	current_selected_character_state_->nickname_ = array[2].character_nick_;
	current_selected_character_state_->cid_= array[2].character_index_; 
	current_selected_character_state_->equipment_state_.arm_item_id = array[2].arm_id_;
	current_selected_character_state_->equipment_state_.body_item_id =array[2].body_id_;
	current_selected_character_state_->equipment_state_.leg_item_id =array[2].leg_id_;
	current_selected_character_state_->equipment_state_.waist_item_id =array[2].waist_id_;
	current_selected_character_state_->equipment_state_.hair_item_id =array[2].hair_id_;
	current_selected_character_state_->equipment_state_.hair_color =array[2].hair_color_;
	current_selected_character_state_->equipment_state_.weapon_type =array[2].weapon_type;
	current_selected_character_state_->equipment_state_.first_weapon_item_id =array[2].weapon_id;
	current_selected_character_->SetPlayerState(current_selected_character_state_);
	current_selected_character_->spring_arm_comp_->SetWorldLocation(FVector(0,75,75));
}
