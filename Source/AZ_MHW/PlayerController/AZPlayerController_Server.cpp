// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_Server.h"

#include "AZ_MHW/Character/Player/AZPlayer_Origin.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "Manager/AZGameCacheInfo.h"


AAZPlayerController_Server::AAZPlayerController_Server()
{
	PrimaryActorTick.bCanEverTick = false;
}

//void AAZPlayerController_Server::BeginPlay()
//{
//	Super::BeginPlay();
//}
//
//void AAZPlayerController_Server::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//	
//}
//
//void AAZPlayerController_Server::OnPossess(APawn* pawn)
//{
//	Super::OnPossess(pawn);
//	
//}
//
//void AAZPlayerController_Server::BeginDestroy()
//{
//	Super::BeginDestroy();
//
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_Server::AddPlayerState_Origin(int32 client_index, int32 character_id)
{
	UE_LOG(LogTemp, Log, TEXT("AddPlayerState_Origin Entered"));
	AAZPlayerState_Client* origin_player_state = nullptr;
	origin_player_state = GetWorld()->SpawnActor<AAZPlayerState_Client>();
	const auto& info =game_instance_->game_cache_info_->GetCharacterSimpleInfo(character_id);
	
	origin_player_state->equipment_state_.hair_color = info.hair_color_;
	origin_player_state->equipment_state_.hair_item_id = info.hair_id_;
	origin_player_state->equipment_state_.head_item_id = info.head_id_;
	origin_player_state->equipment_state_.body_item_id = info.body_id_;
	origin_player_state->equipment_state_.arm_item_id = info.arm_id_;
	origin_player_state->equipment_state_.leg_item_id = info.leg_id_;
	origin_player_state->equipment_state_.waist_item_id = info.waist_id_;
	origin_player_state->equipment_state_.weapon_type = info.weapon_type;
	origin_player_state->equipment_state_.first_weapon_item_id = info.weapon_id;
	origin_player_state->uid_ = client_index;
	
	online_player_character_state_.Add(client_index, origin_player_state);
}

void AAZPlayerController_Server::RemovePlayerState_Origin(int32 client_index)
{
	if(const auto found_player_state_origin = online_player_character_state_.Find(client_index))
	{
		(*found_player_state_origin)->Destroy();
		online_player_character_state_.Remove(client_index);
	}
}

void AAZPlayerController_Server::AddPlayer_Origin(int32 client_index,int32 character_id)
{
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());

	//TODO TEMP임시(인게임 접속전에 호출햇어야함)
	AddPlayerState_Origin(client_index, character_id);

	//원본 캐릭터 생성
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AAZPlayer_Origin* player_origin = GetWorld()->SpawnActor<AAZPlayer_Origin>(FVector(), FRotator(), spawnParams);
	if(const auto player_state_origin = online_player_character_state_.Find(client_index))
	{
		player_origin->SetPlayerState(*player_state_origin);
		player_origin->player_character_state_ = (*player_state_origin);
		player_origin->GetRootComponent()->SetWorldLocation(player_origin->player_character_state_->character_state_.character_position);
		player_origin->GetRootComponent()->SetWorldRotation(player_origin->player_character_state_->character_state_.character_direction);
		online_player_characters_.Add(client_index, player_origin);
	}

	//원본 캐릭터 생성후 로컬 생성 명령
	{
		CREATE_PLAYER_CHARACTER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_CHARACTER_SELECT_RES;
		game_instance_->SendPacketFunc(client_index, sizeof(packet), (char*)&packet);
	}
	
	//접속한 유저가 접속중인 유저를 리모터블로 생성
	for(auto online_player_pair : online_player_characters_)
	{
		const auto& online_player_index = (online_player_pair.Key);
		const auto& online_player = (online_player_pair.Value);
		
		if(client_index != online_player_index)
		{
			INITIALIZE_PLAYER_STATE_PACKET init_packet;
			init_packet.packet_id = (int)PACKET_ID::SC_PLAYER_STATE_REMOTABLE_CREATE_CMD;
			init_packet.guid = online_player_index;//client_index 유저의 remotable_index;
			init_packet.pos = online_player->player_character_state_->character_state_.character_position;
			init_packet.dir = online_player->player_character_state_->character_state_.character_direction.Yaw;

			init_packet.hair_color = online_player->player_character_state_->equipment_state_.hair_color;
			init_packet.head_id = online_player->player_character_state_->equipment_state_.head_item_id;
			init_packet.body_id = online_player->player_character_state_->equipment_state_.body_item_id;
			init_packet.waist_id = online_player->player_character_state_->equipment_state_.waist_item_id;
			init_packet.hair_id = online_player->player_character_state_->equipment_state_.hair_item_id;
			init_packet.arm_id = online_player->player_character_state_->equipment_state_.arm_item_id;
			init_packet.leg_id = online_player->player_character_state_->equipment_state_.leg_item_id;
		
			init_packet.weapon_id = online_player->player_character_state_->equipment_state_.weapon_type;
			init_packet.first_id = online_player->player_character_state_->equipment_state_.first_weapon_item_id;
			init_packet.second_id = online_player->player_character_state_->equipment_state_.second_weapon_item_id;

			game_instance_->SendPacketFunc(client_index, init_packet.packet_length,(char*)&init_packet);

			CREATE_PLAYER_CHARACTER_PACKET packet;
			packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_CREATE_CMD;
			packet.guid = online_player_index;
			
			game_instance_->SendPacketFunc(client_index, packet.packet_length, (char*)&packet);
		}
	}
	
	BroadCast_AddPlayer_Remotable(client_index);
}
void AAZPlayerController_Server::RemovePlayer_Origin(int32 client_index)
{
	if(const auto player_origin = online_player_characters_.Find(client_index))
	{
		online_player_characters_.Remove(client_index);
		(*player_origin)->Destroy();//?

		//서버에서 클라로 해당 id의 원격 캐릭터 제거 명령
	}
	
	BroadCast_RemovePlayer_Remotable(client_index);
}

void AAZPlayerController_Server::ActionPlayer_Origin(int32 client_index, const FVector& cur_pos, float cur_dir, float input_dir, int32 input_data)
{
	//cur_pos이 심하게 차이나면 클라로 ForceInterpolation 전송
	if(const auto origin_player = online_player_characters_.Find(client_index))
	{
	    //강제 위치보간
		const float& interpolation = 50.f;
		const FVector& origin_pos = (*origin_player)->GetRootComponent()->GetComponentLocation();
		ACTION_PLAYER_PACKET packet;
		if(FMath::Abs(origin_pos.X) > (FMath::Abs(cur_pos.X)+FMath::Abs(interpolation)))
		{
			packet.packet_id = (int)PACKET_ID::SC_PLAYER_PLAYABLE_INTERPOLATION_CMD;
			packet.current_position = origin_pos;
			game_instance_->SendPacketFunc(client_index,packet.packet_length,(char*)&packet);
		}
		else if(FMath::Abs(origin_pos.Y) > (FMath::Abs(cur_pos.Y)+FMath::Abs(interpolation)))
		{
			packet.packet_id = (int)PACKET_ID::SC_PLAYER_PLAYABLE_INTERPOLATION_CMD;
			packet.current_position = origin_pos;
			game_instance_->SendPacketFunc(client_index,packet.packet_length,(char*)&packet);
		}
		else if(FMath::Abs(origin_pos.Z) > (FMath::Abs(cur_pos.Z)+FMath::Abs(interpolation)))
		{
			packet.packet_id = (int)PACKET_ID::SC_PLAYER_PLAYABLE_INTERPOLATION_CMD;
			packet.current_position = origin_pos;
			game_instance_->SendPacketFunc(client_index,packet.packet_length,(char*)&packet);
		}
		//원격 플레이어는 이미 서버와 완전 동기화되어 있기 때문에 플레이어블 유저에게만 보간 처리를 하면 된다.
	
		(*origin_player)->player_character_state_->action_state_.input_direction.Yaw = input_dir;
		(*origin_player)->player_character_state_->action_state_.input_bitmask = input_data;
		
		BroadCast_ActionPlayer_Remotable(client_index);
	}
}
void AAZPlayerController_Server::EquipPlayer_Origin(int32 client_index, int32 item_id)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		(*player)->ChangeEquipment(item_id);
		BroadCast_EquipPlayer_Remotable(client_index, item_id);
	}
}

void AAZPlayerController_Server::GesturePlayer_Origin(int32 client_index, int32 gesture_id)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		//(*player)->GetMesh()->GetAnimInstance();
		BroadCast_GesturePlayer_Remotable(client_index, gesture_id);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_Server::BroadCast_AddPlayerState_Remotable(int32 client_index)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		//packet
		
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				//game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}
void AAZPlayerController_Server::BroadCast_RemovePlayerState_Remotable(int32 client_index)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		//packet
		
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				//game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}

void AAZPlayerController_Server::BroadCast_AddPlayer_Remotable(int32 client_index)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		INITIALIZE_PLAYER_STATE_PACKET init_packet;
		init_packet.packet_id = (int)PACKET_ID::SC_PLAYER_STATE_REMOTABLE_CREATE_CMD;
		init_packet.guid = client_index;
		init_packet.pos = (*player)->player_character_state_->character_state_.character_position;
		init_packet.dir = (*player)->player_character_state_->character_state_.character_direction.Yaw;

		init_packet.hair_color = (*player)->player_character_state_->equipment_state_.hair_color;
		init_packet.hair_id = (*player)->player_character_state_->equipment_state_.hair_item_id;
		init_packet.head_id = (*player)->player_character_state_->equipment_state_.head_item_id;
		init_packet.body_id = (*player)->player_character_state_->equipment_state_.body_item_id;
		init_packet.waist_id = (*player)->player_character_state_->equipment_state_.waist_item_id;
		init_packet.arm_id = (*player)->player_character_state_->equipment_state_.arm_item_id;
		init_packet.leg_id = (*player)->player_character_state_->equipment_state_.leg_item_id;
			
		init_packet.weapon_id = (*player)->player_character_state_->equipment_state_.weapon_type;
		init_packet.first_id = (*player)->player_character_state_->equipment_state_.first_weapon_item_id;
		init_packet.second_id = (*player)->player_character_state_->equipment_state_.second_weapon_item_id;

		CREATE_PLAYER_CHARACTER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_CREATE_CMD;
		packet.guid = client_index;
		
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				game_instance_->SendPacketFunc(online_player_index, init_packet.packet_length,(char*)&init_packet);
				game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}
void AAZPlayerController_Server::BroadCast_RemovePlayer_Remotable(int32 client_index)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		//packet
		
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				//game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}

void AAZPlayerController_Server::BroadCast_ActionPlayer_Remotable(int32 client_index)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		ACTION_PLAYER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_ACTION_CMD;
		packet.guid = client_index;
		packet.current_position = (*player)->player_character_state_->character_state_.character_position;
		packet.current_direction = (*player)->player_character_state_->character_state_.character_direction.Yaw;
		
		packet.input_direction = (*player)->player_character_state_->action_state_.input_direction.Yaw;
		packet.input_data = (*player)->player_character_state_->action_state_.input_bitmask;

		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}
void AAZPlayerController_Server::BroadCast_EquipPlayer_Remotable(int32 client_index, int32 item_id)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		EQUIPMENT_PLAYER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_EQUIP_CMD;
		packet.guid = client_index;
		packet.item_id = item_id;

		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}
void AAZPlayerController_Server::BroadCast_GesturePlayer_Remotable(int32 client_index, int32 gesture_id)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		//packet
		GESTURE_PLAYER_PACKET packet;
		packet.guid = client_index;
		packet.gesture_id = gesture_id;
		
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}

void AAZPlayerController_Server::BroadCast_HitPlayer_Remotable(int32 client_index, float angle, int32 damage)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		//packet
		HIT_PLAYER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_HIT_CMD;
		packet.guid = client_index;
		packet.hit_angle = angle;
		packet.damage = damage;
		
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}
void AAZPlayerController_Server::Send_HitPlayer_Playable(int32 client_index, float angle, int32 damage)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		HIT_PLAYER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_PLAYABLE_HIT_CMD;
		packet.guid = client_index;
		packet.hit_angle = angle;
		packet.damage = damage;
		
		//자신에게
		game_instance_->SendPacketFunc(client_index, packet.packet_length, (char*)&packet);
	}
}


//상태변화가 있을때
void AAZPlayerController_Server::BroadCast_UpdatePlayerState_Remotable(int32 client_index, int32 capture_type, int32 capture_value, int32 capture_mask)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
        UPDATE_PLAYER_STATE_PACKET packet;
        packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_UPDATE_CMD;
		packet.state_type = capture_type;
		packet.state_value = capture_value;
		packet.state_bitmask= capture_mask;
		
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			//자신을 제외한 유저들에게만
			if(client_index != online_player_index)
			{
				game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}
void AAZPlayerController_Server::Send_UpdatePlayerState_Playable(int32 client_index, int32 capture_type, int32 capture_value, int32 capture_mask)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		UPDATE_PLAYER_STATE_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_PLAYABLE_UPDATE_CMD;
		packet.state_type = capture_type;
		packet.state_value = capture_value;
		packet.state_bitmask= capture_mask;
		
		//자신에게
		game_instance_->SendPacketFunc(client_index, packet.packet_length, (char*)&packet);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_Server::TempDevelopForceUpdatePlayer_Origin(int32 client_index, FVector pos)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		(*player)->SetActorLocation(pos);
		(*player)->player_character_state_->character_state_.character_position = pos;

		BroadCast_ActionPlayer_Remotable(client_index);
	}
}

