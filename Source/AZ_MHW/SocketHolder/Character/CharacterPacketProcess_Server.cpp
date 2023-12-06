#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "GameInstance/AZGameInstance.h"
#include "PlayerController/AZPlayerController_Server.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"


//CS에 대한
void UPacketFunction::PlayerPlayableCharacterDataRequest(UINT32 client_index)
{
	game_instance_->game_cache_info_->PlayableCharacterDataRequest(client_index);
}
void UPacketFunction::PlayerCharacterCreateRequest(UINT32 client_index, CS_PLAYER_CHARACTER_CREATE_REQ* packet)
{
	game_instance_->game_cache_info_->PlayerCharacterCreateRequest(client_index, packet->create_info);
}

void UPacketFunction::CreatePlayerOriginRequest(UINT32 client_index, CREATE_PLAYER_CHARACTER_PACKET* packet)
{
	auto server_controller =Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	
	server_controller->AddPlayer_Origin(
		client_index,
		packet->character_index);
}


void UPacketFunction::DestroyPlayerOriginRequest(UINT32 client_index)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->RemovePlayer_Origin(
		client_index);
}

void UPacketFunction::ActionPlayerOriginRequest(UINT32 client_index, ACTION_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->ActionPlayer_Origin(
		client_index,
		packet->current_position,
		packet->current_direction,
		packet->input_direction, packet->input_data);
}

void UPacketFunction::EquipPlayerOriginRequest(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->EquipPlayer_Origin(
		client_index,
		packet->item_id);
}

void UPacketFunction::GesturePlayerOriginRequest(UINT32 client_index, GESTURE_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->GesturePlayer_Origin(
		client_index,
		packet->gesture_id);
}

//개발자용 패킷
void UPacketFunction::DevelopPlayerForceUpdateCommand(UINT32 client_index, ACTION_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->TempDevelopForceUpdatePlayer_Origin(
		client_index,
		packet->current_position);
}

