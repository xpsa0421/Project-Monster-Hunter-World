#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "PlayerController/AZPlayerController_InGame.h"
#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterSelect.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "PlayerState/AZPlayerState_Client.h"
#include <Kismet/GameplayStatics.h>

void UPacketFunction::PlayableCharacterDataResponse(SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES* packet)
{
	// 덮어 씌어야 하기 때문에 캐릭터 정보를 리셋한다.
	game_instance_->game_cache_info_->ResetCharacterInfo();
	for (int i = 0; i < packet->count; ++i)
	{
		if (i == 0)
		{
			game_instance_->game_cache_info_->AddCharacterSimpleInfo(packet->info_0);
		}
		if (i == 1)
		{
			game_instance_->game_cache_info_->AddCharacterSimpleInfo(packet->info_1);
		}
		if (i == 2)
		{
			game_instance_->game_cache_info_->AddCharacterSimpleInfo(packet->info_2);
		}
	}

	if (game_instance_->GetHUD()->IsCurScene(EUIName::AZWidget_CharacterSelect))
	{
		if (UAZWidget_CharacterSelect* ui =  game_instance_->GetHUD()->GetUI<UAZWidget_CharacterSelect>(EUIName::AZWidget_CharacterSelect))
		{
			ui->Update();
		}
	}
}

void UPacketFunction::PlayerCharacterCreateResponse(SC_PLAYER_CHARACTER_CREATE_RES* packet)
{
	if (packet->success > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("캐릭터 생성 실패"));
		return;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_CharacterSelect"));
}

void UPacketFunction::CreatePlayerPlayableCommand()
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->AddPlayer_Playable();
}

void UPacketFunction::UpdatePlayerStateCommand(UPDATE_PLAYER_STATE_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->UpdatePlayerState_Playable(
		packet->state_type,
		packet->state_value,
		packet->state_bitmask);
}

void UPacketFunction::InterpolationPlayerPositionCommand(ACTION_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->ForceInterpolationPlayer_Playable(
		packet->current_position);
}


void UPacketFunction::CreatePlayerStateRemotableCommand(INITIALIZE_PLAYER_STATE_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	FAZPlayerCharacterState character_state;
	character_state.character_position = packet->pos;
	character_state.character_direction.Yaw = packet->dir;

	FAZPlayerEquipmentState equipment_state;
	equipment_state.hair_color = packet->hair_color;
	equipment_state.head_item_id = packet->head_id;
	equipment_state.hair_item_id = packet->hair_id;
	equipment_state.arm_item_id = packet->arm_id;
	equipment_state.body_item_id = packet->body_id;
	equipment_state.waist_item_id = packet->waist_id;
	equipment_state.leg_item_id = packet->leg_id;

	equipment_state.weapon_type = packet->weapon_id;
	equipment_state.first_weapon_item_id = packet->first_id;
	equipment_state.second_weapon_item_id = packet->second_id;

	player_controller->AddPlayerState_Remotable(
		packet->guid,
		character_state,
		equipment_state);
}

void UPacketFunction::CreatePlayerRemotableCommand(CREATE_PLAYER_CHARACTER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->AddPlayer_Remotable(
		packet->guid);
}

void UPacketFunction::DestroyPlayerRemotableCommand(DESTROY_PLAYER_CHARACTER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->RemovePlayer_Remotable(
		packet->guid);
}

void UPacketFunction::ActionPlayerRemotableCommand(ACTION_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->ActionPlayer_Remotable(
		packet->guid,
		packet->current_position,
		packet->current_direction,
		packet->input_direction,
		packet->input_data);
}

void UPacketFunction::EquipPlayerRemotableCommand(EQUIPMENT_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->EquipPlayer_Remotable(
		packet->guid,
		packet->item_id);
}

void UPacketFunction::GesturePlayerRemotableCommand(GESTURE_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->GesturePlayer_Remotable(
		packet->guid,
		packet->gesture_id);
}

void UPacketFunction::HitPlayerRemotableHitCommand(HIT_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->HitPlayer_Remotable(
		packet->guid,
		packet->hit_angle,
		packet->damage);
}

void UPacketFunction::HitPlayerPlayableHitCommand(HIT_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->HitPlayer_Playable(
		packet->hit_angle,
		packet->damage);
}

void UPacketFunction::UpdatePlayerStateRemotableCommand(UPDATE_PLAYER_STATE_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->UpdatePlayerState_Remotable(
		packet->guid,
		packet->state_type,
		packet->state_value,
		packet->state_bitmask);
}
