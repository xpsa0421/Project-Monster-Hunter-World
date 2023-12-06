#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Util/AZUtility.h"

void UPacketFunction::Init()
{
	game_instance_ = Cast<UAZGameInstance>(GetOuter());
}

bool UPacketFunction::ProcessPacket(UINT32 client_index, PACKET_HEADER* recv_packet)
{
	bool is_server_packet = true;
	bool is_client_packet = true;
	
	// Process packets from client to server
	switch ((PACKET_ID)recv_packet->packet_id)
	{
#pragma region UI_PART
	case PACKET_ID::CS_LOGIN_SIGNIN_REQ:
		{
			CS_LOGIN_SIGNIN_REQ* packet = (CS_LOGIN_SIGNIN_REQ*)recv_packet;
			UPacketFunction::LoginSigninRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_LOGIN_SIGNUP_REQ:
		{
			CS_LOGIN_SIGNUP_REQ* packet = (CS_LOGIN_SIGNUP_REQ*)recv_packet;
			UPacketFunction::LoginSignupRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_CHAT_MSG_CMD:
		{
			CS_CHAT_MSG_CMD* packet = (CS_CHAT_MSG_CMD*)recv_packet;
			UPacketFunction::ChatMsgCommand(client_index, packet);
		}
		break;
	case PACKET_ID::CS_ITEM_TOTAL_INFO_REQ:
		{
			CS_CHAT_MSG_CMD* packet = (CS_CHAT_MSG_CMD*)recv_packet;
			UPacketFunction::ChatMsgCommand(client_index, packet);
		}
		break;
	case PACKET_ID::CS_ITEM_UNEQUIP_REQ:
		{
			CS_CHAT_MSG_CMD* packet = (CS_CHAT_MSG_CMD*)recv_packet;
			UPacketFunction::ChatMsgCommand(client_index, packet);
		}
		break;
	case PACKET_ID::CS_ITEM_EQUIP_REQ:
		{
			CS_ITEM_EQUIP_REQ* packet = (CS_ITEM_EQUIP_REQ*)recv_packet;
			UPacketFunction::ItemEquipRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_ITEM_MOVE_REQ:
		{
			CS_ITEM_MOVE_REQ* packet = (CS_ITEM_MOVE_REQ*)recv_packet;
			UPacketFunction::ItemMoveRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_ITEM_CREATE_REQ:
		{
			CS_ITEM_CREATE_REQ* packet = (CS_ITEM_CREATE_REQ*)recv_packet;
			UPacketFunction::ItemCreateRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_ITEM_USE_REQ:
		{
			CS_ITEM_USE_REQ* packet = (CS_ITEM_USE_REQ*)recv_packet;
			UPacketFunction::ItemUseRequest(client_index, packet);
		}
		break;
#pragma endregion 
#pragma region Character_PART
#pragma region Called Packet By UI
	case PACKET_ID::CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ:
		{
			//플레이어 캐릭터 데이터 주세요.
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ"));
			UPacketFunction::PlayerPlayableCharacterDataRequest(client_index);
			//없으니까 무시
		}
		break;
	case PACKET_ID::CS_PLAYER_CHARACTER_CREATE_REQ:
		{
			CS_PLAYER_CHARACTER_CREATE_REQ* packet = (CS_PLAYER_CHARACTER_CREATE_REQ*)recv_packet;
			UPacketFunction::PlayerCharacterCreateRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_PLAYER_PLAYABLE_CHARACTER_DESTROY_REQ:
		{
			//플레이어 캐릭터 데이터를 제거해주세요.
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_PLAYER_PLAYABLE_CHARACTER_DESTROY_REQ"));
			//UPacketFunction::
		}
		break;
	case PACKET_ID::CS_PLAYER_ORIGIN_CREATE_REQ:
		{
			//인 게임에 접속햇으니 서버에 생성해주세요.
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_PLAYER_ORIGIN_CREATE_REQ"));
			CREATE_PLAYER_CHARACTER_PACKET* packet = (CREATE_PLAYER_CHARACTER_PACKET*)recv_packet;
			UPacketFunction::CreatePlayerOriginRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_PLAYER_ORIGIN_EQUIP_REQ:
		{
			//장비변경햇으니 변경하세요.
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_PLAYER_ORIGIN_EQUIP_REQ"));
			EQUIPMENT_PLAYER_PACKET* packet = (EQUIPMENT_PLAYER_PACKET*)recv_packet;
			UPacketFunction::EquipPlayerOriginRequest(client_index,packet);
		}
		break;
#pragma endregion
		/////////////////////////////////////////////////////////////
	case PACKET_ID::CS_PLAYER_ORIGIN_DESTROY_REQ:
		{
			//접속종료햇으니 서버에서 제거하세요.
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_PLAYER_ORIGIN_DESTROY_REQ"));
			UPacketFunction::DestroyPlayerOriginRequest(client_index);
		}
		break;
	case PACKET_ID::CS_PLAYER_ORIGIN_ACTION_REQ:
		{
			//입력햇으니 액션하세요.
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_PLAYER_ORIGIN_ACTION_REQ"));
			ACTION_PLAYER_PACKET* packet = (ACTION_PLAYER_PACKET*)recv_packet;
			UPacketFunction::ActionPlayerOriginRequest(client_index, packet);
		}
		break;
	case PACKET_ID::CS_DEVELOP_PLAYER_FORCE_UPDATE_CMD:
		{
			//개발자용 클라 투 서버 동기화 패킷
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_DEVELOP_PLAYER_FORCE_UPDATE_CMD"));
			ACTION_PLAYER_PACKET* packet = (ACTION_PLAYER_PACKET*)recv_packet;
			UPacketFunction::DevelopPlayerForceUpdateCommand(client_index, packet);
		}
		break;
	case PACKET_ID::CS_PLAYER_ORIGIN_GESTURE_REQ:
		{
			UE_LOG(AZ_PLAYER,Warning,TEXT("CS_PLAYER_ORIGIN_GESTURE_REQ"));
			GESTURE_PLAYER_PACKET* packet = (GESTURE_PLAYER_PACKET*)recv_packet;
			UPacketFunction::GesturePlayerOriginRequest(client_index, packet);
		}
		break;
#pragma endregion
#pragma region Monster_PART
	case PACKET_ID::CS_COMBAT_MAP_ENTER_REQ:
	{
		CS_COMBAT_MAP_ENTER_REQ* packet = (CS_COMBAT_MAP_ENTER_REQ*)recv_packet;
		UPacketFunction::Receive_CS_COMBAT_MAP_ENTER_REQ(client_index, packet);
	}
	break;
	case PACKET_ID::CS_COMBAT_MAP_LOAD_FINISH_CMD:
	{
		CS_COMBAT_MAP_LOAD_FINISH_CMD* packet = (CS_COMBAT_MAP_LOAD_FINISH_CMD*)recv_packet;
		UPacketFunction::Receive_CS_COMBAT_MAP_LOAD_FINISH_CMD(client_index, packet);
	}
	break;
	case PACKET_ID::CS_MONSTER_UPDATE_REQ:
	{
		CS_MONSTER_UPDATE_REQ* packet = (CS_MONSTER_UPDATE_REQ*)recv_packet;
		UPacketFunction::Receive_CS_MONSTER_UPDATE_REQ(client_index, packet);
	}
	break;
#pragma endregion 
	default:
		{
			is_server_packet = false;
		}
		break;
	}
//===========================================================================================================================================//
	// Process packets from server to client
	FString out_request_protocol("");
	switch ((PACKET_ID)recv_packet->packet_id)
	{
#pragma region UI_PART
	case PACKET_ID::SC_LOGIN_SIGNIN_RES:
		{
			SC_LOGIN_SIGNIN_RES* packet = (SC_LOGIN_SIGNIN_RES*)recv_packet;
			UPacketFunction::LoginSigninResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_LOGIN_SIGNIN_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_LOGIN_SIGNUP_RES:
		{
			SC_LOGIN_SIGNUP_RES* packet = (SC_LOGIN_SIGNUP_RES*)recv_packet;
			UPacketFunction::LoginSignupResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_LOGIN_SIGNUP_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_CHAT_MSG_CMD:
		{
			SC_CHAT_MSG_CMD* packet = (SC_CHAT_MSG_CMD*)recv_packet;
			UPacketFunction::ChatMsgCommand(packet);
		}
		break;
	case PACKET_ID::SC_ITEM_TOTAL_INFO_RES:
		{
			SC_ITEM_TOTAL_INFO_RES* packet = (SC_ITEM_TOTAL_INFO_RES*)recv_packet;
			UPacketFunction::ItemTotalInfoResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_ITEM_TOTAL_INFO_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_ITEM_UNEQUIP_RES:
		{
			SC_ITEM_UNEQUIP_RES* packet = (SC_ITEM_UNEQUIP_RES*)recv_packet;
			UPacketFunction::ItemUnEquipResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_ITEM_UNEQUIP_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_ITEM_EQUIP_RES:
		{
			SC_ITEM_EQUIP_RES* packet = (SC_ITEM_EQUIP_RES*)recv_packet;
			UPacketFunction::ItemEquipResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_ITEM_EQUIP_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_ITEM_MOVE_RES:
		{
			SC_ITEM_MOVE_RES* packet = (SC_ITEM_MOVE_RES*)recv_packet;
			UPacketFunction::ItemMoveResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_ITEM_MOVE_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_ITEM_CREATE_RES:
		{
			SC_ITEM_CREATE_RES* packet = (SC_ITEM_CREATE_RES*)recv_packet;
			UPacketFunction::ItemCreateResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_ITEM_CREATE_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_ITEM_USE_RES:
		{
			SC_ITEM_USE_RES* packet = (SC_ITEM_USE_RES*)recv_packet;
			UPacketFunction::ItemUseResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_ITEM_USE_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_ITEM_INFO_CMD:
		{
			SC_ITEM_INFO_CMD* packet = (SC_ITEM_INFO_CMD*)recv_packet;
			UPacketFunction::ItemInfoCommand(packet);
		}
		break;
#pragma endregion 
#pragma region Character_PART
#pragma region Update_Field
	case PACKET_ID::SC_ENVIRONMENT_UPDATE_MERCHANT_CMD:
		{
			//상점 갱신 명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_ENVIRONMENT_UPDATE_MERCHANT_CMD"));
		}
		break;
	case PACKET_ID::SC_ENVIRONMENT_UPDATE_FIELD_CMD:
		{
			//필드 갱신 명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_ENVIRONMENT_UPDATE_FIELD_CMD"));
		}
		break;
#pragma endregion
	case PACKET_ID::SC_PLAYER_STATE_REMOTABLE_CREATE_CMD:
		{
			//원격 데이터생성명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_REMOTABLE_STATE_CMD"))
			INITIALIZE_PLAYER_STATE_PACKET* packet = (INITIALIZE_PLAYER_STATE_PACKET*)recv_packet;
			UPacketFunction::CreatePlayerStateRemotableCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_CREATE_CMD:
		{
			//원격 생성명령
			UE_LOG(AZ_PLAYER, Warning, TEXT("SC_PLAYER_REMOTABLE_CREATE_CMD"));
			CREATE_PLAYER_CHARACTER_PACKET* packet = (CREATE_PLAYER_CHARACTER_PACKET*) recv_packet;
			UPacketFunction::CreatePlayerRemotableCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_DESTROY_CMD:
		{
			//원격 제거명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_REMOTABLE_DESTROY_CMD"));
			DESTROY_PLAYER_CHARACTER_PACKET* packet = (DESTROY_PLAYER_CHARACTER_PACKET*) recv_packet;
			UPacketFunction::DestroyPlayerRemotableCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_EQUIP_CMD:
		{
			//원격 장비명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_REMOTABLE_EQUIP_CMD"));
			EQUIPMENT_PLAYER_PACKET* packet = (EQUIPMENT_PLAYER_PACKET*)recv_packet;
			UPacketFunction::EquipPlayerRemotableCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_ACTION_CMD:
		{
			//원격 액션명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_REMOTABLE_ACTION_CMD"));
			ACTION_PLAYER_PACKET* packet = (ACTION_PLAYER_PACKET*) recv_packet;
			UPacketFunction::ActionPlayerRemotableCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_UPDATE_CMD:
		{
			//원격 갱신명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_REMOTABLE_UPDATE_CMD"));
			UPDATE_PLAYER_STATE_PACKET* packet = (UPDATE_PLAYER_STATE_PACKET*) recv_packet;
			UPacketFunction::UpdatePlayerStateRemotableCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_PLAYABLE_UPDATE_CMD:
		{
			//플레이어 갱신명령
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_PLAYABLE_UPDATE_CMD"));
			UPDATE_PLAYER_STATE_PACKET* packet = (UPDATE_PLAYER_STATE_PACKET*) recv_packet;
			UPacketFunction::UpdatePlayerStateCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_PLAYABLE_INTERPOLATION_CMD:
		{
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_PLAYABLE_INTERPOLATION_CMD"));
			ACTION_PLAYER_PACKET* packet = (ACTION_PLAYER_PACKET*) recv_packet;
			UPacketFunction::InterpolationPlayerPositionCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_GESTURE_CMD:
		{
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_REMOTABLE_GESTURE_CMD"));
			GESTURE_PLAYER_PACKET* packet = (GESTURE_PLAYER_PACKET*) recv_packet;
			UPacketFunction::GesturePlayerRemotableCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_PLAYABLE_HIT_CMD:
		{
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_PLAYABLE_HIT_CMD"));
			HIT_PLAYER_PACKET* packet = (HIT_PLAYER_PACKET*) recv_packet;
			UPacketFunction::HitPlayerPlayableHitCommand(packet);
		}
		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_HIT_CMD:
		{
			UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_REMOTABLE_HIT_CMD"));
			HIT_PLAYER_PACKET* packet = (HIT_PLAYER_PACKET*) recv_packet;
			UPacketFunction::HitPlayerRemotableHitCommand(packet);
		}
		break;
/////////////////////////////////////////////////////////////////////////////////////////
/// 캐릭터 데이터 (로그인 창)
/////////////////////////////////////////////////////////////////////////////////////////
	case PACKET_ID::SC_PLAYER_CHARACTER_CREATE_RES:
		{
			SC_PLAYER_CHARACTER_CREATE_RES* packet = (SC_PLAYER_CHARACTER_CREATE_RES*)recv_packet;
			UPacketFunction::PlayerCharacterCreateResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_PLAYER_CHARACTER_CREATE_REQ, out_request_protocol);		}
		break;
	case PACKET_ID::SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES:
		{
			//UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES"));
			//캐릭터 선택한 데이터 받기
			SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES* packet = (SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES*)recv_packet;
			UPacketFunction::PlayableCharacterDataResponse(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_PLAYER_PLAYABLE_CHARACTER_DESTROY_RES:
		{
			//캐릭터 파괴 데이터 받기
			//UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_PLAYABLE_CHARACTER_DESTROY_RES"));
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_PLAYER_PLAYABLE_CHARACTER_DESTROY_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_PLAYER_CHARACTER_SELECT_RES:
		{
			//캐릭터 선택에 대한 서버의 응답(원격 생성후 로컬 생성 허락)
			//캐릭터 선택을 했으니 로컬에 생성하라
			//UE_LOG(AZ_PLAYER,Warning,TEXT("SC_PLAYER_CHARACTER_SELECT_RES"));
			UPacketFunction::CreatePlayerPlayableCommand();
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(
				PACKET_ID::CS_PLAYER_CHARACTER_SELECT_REQ, out_request_protocol);
		}
		break;
#pragma endregion
#pragma region Monster_PART
	case PACKET_ID::SC_MONSTER_SPAWN_CMD:
		{
			const SC_MONSTER_SPAWN_CMD* packet = reinterpret_cast<SC_MONSTER_SPAWN_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_SPAWN_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_SPAWN_END_CMD:
		{
			const SC_MONSTER_SPAWN_END_CMD* packet = reinterpret_cast<SC_MONSTER_SPAWN_END_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_SPAWN_END_CMD(packet);
			game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_COMBAT_MAP_ENTER_REQ, out_request_protocol);
		}
		break;
	case PACKET_ID::SC_MONSTER_TRANSFORM_CMD:
		{
			const SC_MONSTER_TRANSFORM_CMD* packet = reinterpret_cast<SC_MONSTER_TRANSFORM_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_TRANSFORM_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_BODY_STATE_CMD:
		{
			const SC_MONSTER_BODY_STATE_CMD* packet = reinterpret_cast<SC_MONSTER_BODY_STATE_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_BODY_STATE_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_ENTER_COMBAT_CMD:
		{
			const SC_MONSTER_ENTER_COMBAT_CMD* packet = reinterpret_cast<SC_MONSTER_ENTER_COMBAT_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_ENTER_COMBAT_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_ACTION_START_CMD:
		{
			const SC_MONSTER_ACTION_START_CMD* packet = reinterpret_cast<SC_MONSTER_ACTION_START_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_ACTION_START_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_ACTION_END_CMD:
		{
			const SC_MONSTER_ACTION_END_CMD* packet = reinterpret_cast<SC_MONSTER_ACTION_END_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_ACTION_END_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_PART_CHANGE_CMD:
		{
			const SC_MONSTER_PART_CHANGE_CMD* packet = reinterpret_cast<SC_MONSTER_PART_CHANGE_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_PART_CHANGE_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_HIT_CMD:
		{
			const SC_MONSTER_HIT_CMD* packet = reinterpret_cast<SC_MONSTER_HIT_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_HIT_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_ENRAGE_BEGIN_CMD:
		{
			const SC_MONSTER_ENRAGE_BEGIN_CMD* packet = reinterpret_cast<SC_MONSTER_ENRAGE_BEGIN_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_ENRAGE_BEGIN_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_ENRAGE_END_CMD:
		{
			const SC_MONSTER_ENRAGE_END_CMD* packet = reinterpret_cast<SC_MONSTER_ENRAGE_END_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_ENRAGE_END_CMD(packet);
		}
		break;
	case PACKET_ID::SC_MONSTER_DIE_CMD:
		{
			const SC_MONSTER_DIE_CMD* packet = reinterpret_cast<SC_MONSTER_DIE_CMD*>(recv_packet);
			UPacketFunction::Receive_SC_MONSTER_DIE_CMD(packet);
		}
		break;
#pragma endregion 
	default:
		{
			is_client_packet = false;
		}
		break;
	}

	if ((is_server_packet || is_client_packet) == false)
	{
		AZ_LOG("Packet Not Process : id : %d", recv_packet->packet_id);
	}

	game_instance_->GetSocketHolder(ESocketHolderType::Game)->ScreenWaitProc(out_request_protocol);

	return (is_server_packet || is_client_packet);
}
