#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "Character/Monster/AZMonster.h"
#include "PacketFunction.generated.h"


class AAZMonster_Client;

UCLASS()
class AZ_MHW_API UPacketFunction : public UObject
{
	// static 함수로 만들시 gameinstance의 종류를 찾지를 못함(인스턴스 2개 실행시)
	GENERATED_BODY()

public:
	UPacketFunction() {}

public:
	bool ProcessPacket(UINT32 client_index, PACKET_HEADER* recv_packet);
	void Init();

public:
	// server(Login)
	void LoginSigninRequest(UINT32 client_index, CS_LOGIN_SIGNIN_REQ* packet);
	void LoginSignupRequest(UINT32 client_index, CS_LOGIN_SIGNUP_REQ* packet);
	// client(Login)
	void LoginSigninResponse(SC_LOGIN_SIGNIN_RES* packet);
	void LoginSignupResponse(SC_LOGIN_SIGNUP_RES* packet);

public:
	// server(chat)
	void ChatMsgCommand(UINT32 client_index, CS_CHAT_MSG_CMD* packet);
	// client(chat)
	void ChatMsgCommand(SC_CHAT_MSG_CMD* packet);

public:
	// server(item)
	void ItemTotalInfoRequest(UINT32 client_index, CS_ITEM_TOTAL_INFO_REQ* packet);
	void ItemUnequipRequest(UINT32 client_index, CS_ITEM_UNEQUIP_REQ* packet);
	void ItemEquipRequest(UINT32 client_index, CS_ITEM_EQUIP_REQ* packet);
	void ItemMoveRequest(UINT32 client_index, CS_ITEM_MOVE_REQ* packet);
	void ItemCreateRequest(UINT32 client_index, CS_ITEM_CREATE_REQ* packet);
	void ItemUseRequest(UINT32 client_index, CS_ITEM_USE_REQ* packet);

	// client(item)
	void ItemTotalInfoResponse(SC_ITEM_TOTAL_INFO_RES* packet);
	void ItemUnEquipResponse(SC_ITEM_UNEQUIP_RES* packet);
	void ItemEquipResponse(SC_ITEM_EQUIP_RES* packet);
	void ItemMoveResponse(SC_ITEM_MOVE_RES* packet);
	void ItemCreateResponse(SC_ITEM_CREATE_RES* packet);
	void ItemUseResponse(SC_ITEM_USE_RES* packet);
	void ItemInfoCommand(SC_ITEM_INFO_CMD* packet);

	// Monster
	AAZMonster_Client* GetMonster_Client(int32 object_serial);
	void Receive_SC_MONSTER_SPAWN_CMD(const SC_MONSTER_SPAWN_CMD* packet);
	void Receive_SC_MONSTER_SPAWN_END_CMD(const SC_MONSTER_SPAWN_END_CMD* packet);
	void Receive_SC_MONSTER_TRANSFORM_CMD(const SC_MONSTER_TRANSFORM_CMD* packet);
	void Receive_SC_MONSTER_BODY_STATE_CMD(const SC_MONSTER_BODY_STATE_CMD* packet);
	void Receive_SC_MONSTER_ENTER_COMBAT_CMD(const SC_MONSTER_ENTER_COMBAT_CMD* packet);
	void Receive_SC_MONSTER_ACTION_START_CMD(const SC_MONSTER_ACTION_START_CMD* packet);
	void Receive_SC_MONSTER_ACTION_END_CMD(const SC_MONSTER_ACTION_END_CMD* packet);
	void Receive_SC_MONSTER_PART_CHANGE_CMD(const SC_MONSTER_PART_CHANGE_CMD* packet);
	void Receive_SC_MONSTER_HIT_CMD(const SC_MONSTER_HIT_CMD* packet);
	void Receive_SC_MONSTER_ENRAGE_BEGIN_CMD(const SC_MONSTER_ENRAGE_BEGIN_CMD* packet);
	void Receive_SC_MONSTER_ENRAGE_END_CMD(const SC_MONSTER_ENRAGE_END_CMD* packet);
	void Receive_SC_MONSTER_DIE_CMD(const SC_MONSTER_DIE_CMD* packet);

	AAZMonster* GetMonster_Server(int32 object_serial);
	void Receive_CS_COMBAT_MAP_ENTER_REQ(UINT32 client_index, CS_COMBAT_MAP_ENTER_REQ* packet);
	void Receive_CS_COMBAT_MAP_LOAD_FINISH_CMD(UINT32 client_index, CS_COMBAT_MAP_LOAD_FINISH_CMD* packet);
	void TimerLoop_Send_SC_MONSTER_SPAWN_CMD(UINT32 client_idx, int32 num_to_spawn);
	void Receive_CS_MONSTER_UPDATE_REQ(UINT32 client_idx, CS_MONSTER_UPDATE_REQ* packet);

#pragma region PlayerCharacterParts
public:
#pragma region Client->Server
	void PlayerPlayableCharacterDataRequest(UINT32 client_index);
	void PlayerCharacterCreateRequest(UINT32 client_index, CS_PLAYER_CHARACTER_CREATE_REQ* packet);

	//void CreatePlayerCharacterRequest(UINT32 client_index);
	//void DestroyPlayerCharacterRequest(UINT32 client_index);
	
	void CreatePlayerOriginRequest(UINT32 client_index, CREATE_PLAYER_CHARACTER_PACKET* packet);
	void DestroyPlayerOriginRequest(UINT32 client_index);

	void ActionPlayerOriginRequest(UINT32 client_index, ACTION_PLAYER_PACKET* packet);
	void EquipPlayerOriginRequest(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet);
	void GesturePlayerOriginRequest(UINT32 client_index, GESTURE_PLAYER_PACKET* packet);

	void DevelopPlayerForceUpdateCommand(UINT32 client_index, ACTION_PLAYER_PACKET* packet);
#pragma endregion
#pragma region Server->Client
	void PlayableCharacterDataResponse(SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES* packet);
	void PlayerCharacterCreateResponse(SC_PLAYER_CHARACTER_CREATE_RES* packet);

	//void CreatePlayerCharacterRespone();//플레이어 생성후 데이터받기
	//void DestroyPlayerCharacterRespone();//플레이어 제거후 데이터받기
	
	void CreatePlayerPlayableCommand();
	void CreatePlayerStateRemotableCommand(INITIALIZE_PLAYER_STATE_PACKET* packet);
	
	void CreatePlayerRemotableCommand(CREATE_PLAYER_CHARACTER_PACKET* packet);
	void DestroyPlayerRemotableCommand(DESTROY_PLAYER_CHARACTER_PACKET* packet);

	void ActionPlayerRemotableCommand(ACTION_PLAYER_PACKET* packet);
	void EquipPlayerRemotableCommand(EQUIPMENT_PLAYER_PACKET* packet);
	void GesturePlayerRemotableCommand(GESTURE_PLAYER_PACKET* packet);
	void HitPlayerRemotableHitCommand(HIT_PLAYER_PACKET* packet);
	

	void UpdatePlayerStateCommand(UPDATE_PLAYER_STATE_PACKET* packet);
	void UpdatePlayerStateRemotableCommand(UPDATE_PLAYER_STATE_PACKET* packet);
	void InterpolationPlayerPositionCommand(ACTION_PLAYER_PACKET* packet);
	void HitPlayerPlayableHitCommand(HIT_PLAYER_PACKET* packet);
	
	void DisplayPlayerDemageRequest();//플레이어가 준 데미지 표시
#pragma endregion 

private:
	UPROPERTY() TWeakObjectPtr<class UAZGameInstance> game_instance_;
	FTimerHandle test_timer;
};
