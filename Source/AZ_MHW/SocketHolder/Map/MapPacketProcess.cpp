#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "Character/Monster/AZMonster.h"
#include "CharacterComponent/AZMonsterPacketHandlerComponent.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_Server.h"
#include "Manager/AZObjectMgr_Server.h"

// TEMP
void UPacketFunction::Receive_CS_COMBAT_MAP_ENTER_REQ(UINT32 client_index, CS_COMBAT_MAP_ENTER_REQ* packet)
{
	UE_LOG(AZMonster_Network, Warning, TEXT("Receive_CS_COMBAT_MAP_ENTER_REQ"));

	// Spawn monsters on the map if first enter
	UAZObjectMgr_Server* object_mgr = Cast<AAZGameMode_Server>(game_instance_->GetGameMode())->object_mgr_;
	if (!object_mgr->spawn_finished_)
	{
		object_mgr->SpawnAllMonsters();
	}
}

// TEMP
// 맵로딩 끝났으면 클라이언트에 몬스터 스폰
void UPacketFunction::Receive_CS_COMBAT_MAP_LOAD_FINISH_CMD(UINT32 client_index, CS_COMBAT_MAP_LOAD_FINISH_CMD* packet)
{
	UE_LOG(AZMonster_Network, Warning, TEXT("Receive_CS_COMBAT_MAP_LOAD_FINISH_CMD"));

	// Spawn monsters on the newly joined client side
	auto object_mgr = Cast<AAZGameMode_Server>(game_instance_->GetGameMode())->object_mgr_;
	for (auto monster : object_mgr->spawned_monsters_array_)
	{
		monster->packet_handler_component_->Send_SC_MONSTER_SPAWN_CMD(client_index);
	}

	// Send end spawn packet
	SC_MONSTER_SPAWN_END_CMD end_packet;	
	game_instance_->SendPacketFunc(client_index, end_packet.packet_length, reinterpret_cast<char*>(&end_packet));
	UE_LOG(AZMonster_Network, Log, TEXT("SC_MONSTER_SPAWN_END_CMD"));
}

// deprecated
void UPacketFunction::TimerLoop_Send_SC_MONSTER_SPAWN_CMD(UINT32 client_index, int32 num_to_spawn)
{
	static int monster_index = 0;

	if (monster_index == num_to_spawn)
	{
		game_instance_->GetWorld()->GetTimerManager().ClearTimer(test_timer);
		// Send end spawn packet
		SC_MONSTER_SPAWN_END_CMD end_packet;	
		game_instance_->SendPacketFunc(client_index, end_packet.packet_length, reinterpret_cast<char*>(&end_packet));
		UE_LOG(AZMonster_Network, Log, TEXT("SC_MONSTER_SPAWN_END_CMD"));
		monster_index = 0;
	}
	else
	{
		auto array = Cast<AAZGameMode_Server>(game_instance_->GetGameMode())->object_mgr_->spawned_monsters_array_;
		array[monster_index]->packet_handler_component_->Send_SC_MONSTER_SPAWN_CMD(client_index);
		monster_index++;
	}
}


