#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameMode/AZGameMode_InGame.h"
#include "AZ_MHW/Manager/AZObjectMgr_Client.h"
#include "CharacterComponent/AZMonsterPacketHandlerComponent_Client.h"
#include "Engine/LevelStreamingDynamic.h"
#include "PlayerController/AZPlayerController_InGame.h"


AAZMonster_Client* UPacketFunction::GetMonster_Client(int32 object_serial)
{
	const auto gamemode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode());
	if (!gamemode)
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][GetMonster_Client] Incorrect gamemode!"));
		return nullptr;
	}
	return gamemode->object_mgr_->GetMonster(object_serial);
}

void UPacketFunction::Receive_SC_MONSTER_SPAWN_CMD(const SC_MONSTER_SPAWN_CMD* packet)
{
	UE_LOG(AZMonster_Network, Warning, TEXT("Receive_SC_MONSTER_SPAWN_CMD"));
	Cast<AAZGameMode_InGame>(game_instance_->GetGameMode())->object_mgr_->spawn_packets_.Enqueue(*packet);
}

void UPacketFunction::Receive_SC_MONSTER_SPAWN_END_CMD(const SC_MONSTER_SPAWN_END_CMD* packet)
{
	UE_LOG(AZMonster_Network, Warning, TEXT("Receive_SC_MONSTER_SPAWN_END_CMD"));
	auto gamemode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode());
	gamemode->object_mgr_->SpawnAllMonsters();
	gamemode->combat_level_->SetShouldBeVisible(true);

	// TODO TEMP 플레이어로직, 플레이어 안의 함수로 빼기 / 실제로는 시네마틱 끝나고 이동해야할듯
	const auto& player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->playable_player_->SetActorLocation(FVector(80678,41570,4261));
	player_controller->playable_player_->SetActorRotation(FRotator(0,-180,0));
	player_controller->TempSendForceUpdatePlayer_Origin();
	
	// TODO Warp 서버에 패킷 보내서 강제 이동 브로드캐스트 or 서버에서 이동 요청 
}

void UPacketFunction::Receive_SC_MONSTER_TRANSFORM_CMD(const SC_MONSTER_TRANSFORM_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;

	target_monster->packet_handler_component_->Receive_SC_MONSTER_TRANSFORM_CMD(packet->location, packet->rotation, packet->is_forced);
}

void UPacketFunction::Receive_SC_MONSTER_BODY_STATE_CMD(const SC_MONSTER_BODY_STATE_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;

	target_monster->packet_handler_component_->Receive_SC_MONSTER_BODY_STATE_CMD(
		packet->head_state, packet->back_state, packet->left_wing_state, packet->right_wing_state, packet->tail_state);
}

void UPacketFunction::Receive_SC_MONSTER_ENTER_COMBAT_CMD(const SC_MONSTER_ENTER_COMBAT_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_ENTER_COMBAT_CMD();
}

void UPacketFunction::Receive_SC_MONSTER_ACTION_START_CMD(const SC_MONSTER_ACTION_START_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;

	MonsterActionStateInfo state_info_temp = packet->state_info;
	const FMonsterActionStateInfo state_info = state_info_temp.Convert();
	
	UE_LOG(AZMonster_Network, Log, TEXT("[Receive_SC_MONSTER_ACTION_START_CMD] Action mode: %s, Animation name: %s, Montage name: %s"),
		*UAZUtility::EnumToString(state_info.action_mode), *state_info.animation_name.ToString(), *state_info.montage_section_name.ToString());
	target_monster->packet_handler_component_->Receive_SC_MONSTER_ACTION_START_CMD(state_info, packet->start_position);
}

void UPacketFunction::Receive_SC_MONSTER_ACTION_END_CMD(const SC_MONSTER_ACTION_END_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;

	EMonsterActionMode action_mode = packet->action_mode;
	FVector location = packet->location;
	FRotator rotation = packet->rotation;
	target_monster->packet_handler_component_->Receive_SC_MONSTER_ACTION_END_CMD(location, rotation, action_mode);
}

void UPacketFunction::Receive_SC_MONSTER_PART_CHANGE_CMD(const SC_MONSTER_PART_CHANGE_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_PART_CHANGE_CMD(packet->body_part, packet->change_type);
}

void UPacketFunction::Receive_SC_MONSTER_HIT_CMD(const SC_MONSTER_HIT_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_HIT_CMD(packet->hit_info);
}

void UPacketFunction::Receive_SC_MONSTER_ENRAGE_BEGIN_CMD(const SC_MONSTER_ENRAGE_BEGIN_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_ENRAGE_BEGIN_CMD();
}

void UPacketFunction::Receive_SC_MONSTER_ENRAGE_END_CMD(const SC_MONSTER_ENRAGE_END_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_ENRAGE_END_CMD();
}

void UPacketFunction::Receive_SC_MONSTER_DIE_CMD(const SC_MONSTER_DIE_CMD* packet)
{
	const auto gamemode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode());
	if (!gamemode)
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][Receive_SC_MONSTER_SPAWN_CMD] Incorrect gamemode!"));
		return;
	}

	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (target_monster)
		target_monster->packet_handler_component_->Receive_SC_MONSTER_DIE_CMD();
	
	if (gamemode->object_mgr_->RemoveObject(packet->object_serial))
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_DIE_CMD] %d removal succeeded"), packet->object_serial);
	}
	else
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_DIE_CMD] %d removal failed"), packet->object_serial);
	}
}

