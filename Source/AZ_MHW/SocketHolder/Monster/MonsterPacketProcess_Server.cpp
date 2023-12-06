#include "CharacterComponent/AZMonsterPacketHandlerComponent.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_Server.h"
#include "Manager/AZObjectMgr_Server.h"
#include "SocketHolder/PacketFunction.h"

AAZMonster* UPacketFunction::GetMonster_Server(int32 object_serial)
{
	const auto gamemode = Cast<AAZGameMode_Server>(game_instance_->GetGameMode());
	if (!gamemode)
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][GetMonster_Server] Incorrect gamemode!"));
		return nullptr;
	}
	return gamemode->object_mgr_->GetMonster(object_serial);
}

void UPacketFunction::Receive_CS_MONSTER_UPDATE_REQ(UINT32 client_index, CS_MONSTER_UPDATE_REQ* packet)
{
	UE_LOG(AZMonster_Network, Warning, TEXT("[Client %d] Receive_CS_MONSTER_UPDATE_REQ for monster [%d]"), client_index, packet->object_serial);
	AAZMonster* monster = GetMonster_Server(packet->object_serial);
	monster->packet_handler_component_->Receive_CS_MONSTER_UPDATE_REQ(client_index);
}
