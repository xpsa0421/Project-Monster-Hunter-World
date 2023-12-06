// Copyright Team AZ. All Rights Reserved.

#include "AZMonsterPacketHandlerComponent_Client.h"

#include "AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "Character/Player/AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SocketHolder/AZSocketHolder.h"

UAZMonsterPacketHandlerComponent_Client::UAZMonsterPacketHandlerComponent_Client()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAZMonsterPacketHandlerComponent_Client::Init()
{
	// Set owner as monster_client
	owner_ = Cast<AAZMonster_Client>(GetOwner());
	if (!owner_)
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterPacketHandlerComponent_Client] Invalid owner actor!"));
	}
	game_instance_ = Cast<UAZGameInstance>(GetOwner()->GetWorld()->GetGameInstance());
}

void UAZMonsterPacketHandlerComponent_Client::Send_CS_MONSTER_UPDATE_REQ()
{
	CS_MONSTER_UPDATE_REQ end_packet;
	end_packet.object_serial = owner_->object_serial_;
	end_packet.packet_length = sizeof(end_packet);
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&end_packet, end_packet.packet_length);
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][%d] Send_CS_MONSTER_UPDATE_REQ"), owner_->object_serial_);
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_TRANSFORM_CMD(FVector location, FRotator rotation, bool is_forced)
{
	if (is_forced)
	{
		owner_->SetActorLocation(location);
		owner_->SetActorRotation(rotation);
	}
	else
	{
		// TODO need interp
		owner_->SetActorLocation(location);
		owner_->SetActorRotation(rotation);
	}

	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][%d][Receive_SC_MONSTER_TRANSFORM_CMD] %s, Location: %s, Rotation %s"),
		owner_->object_serial_, is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *location.ToString(), *rotation.ToString());
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_BODY_STATE_CMD(
	const FBossBodyPartDebuffState head_state, const FBossBodyPartDebuffState back_state, const FBossBodyPartDebuffState left_wing_state,
	const FBossBodyPartDebuffState right_wing_state, const FBossBodyPartDebuffState tail_state)
{
	// handle in mesh component
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Head, head_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Back, back_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::LeftWing, left_wing_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::RightWing, right_wing_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Tail, tail_state);

	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][#%d][Receive_SC_MONSTER_BODY_STATE_CMD]"), owner_->object_serial_);
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_ENTER_COMBAT_CMD()
{
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][#%d][Receive_SC_MONSTER_ENTER_COMBAT_CMD]"), owner_->object_serial_);
	owner_->OnEnterCombat.Broadcast();
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_ACTION_START_CMD(const FMonsterActionStateInfo action_info, float start_position)
{
	owner_->anim_instance_->is_doing_action_ = false;
	owner_->SetActionStateInfo(action_info);
	owner_->anim_instance_->UpdateAnimation();
	//if (start_position != 0) start_position = owner_->GetWorld()->GetDeltaSeconds() * 4;
	owner_->anim_instance_->start_position_ = start_position;
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_ACTION_END_CMD(FVector location, FRotator rotation, EMonsterActionMode action_mode)
{
	owner_->SetActorLocation(location);
	owner_->SetActorRotation(rotation);
	owner_->action_state_info_.action_mode = action_mode;
	owner_->anim_instance_->is_doing_action_ = false;
	owner_->action_state_info_.priority_score = EMonsterActionPriority::Locomotion;
	owner_->action_state_info_.animation_name = NAME_None;
	owner_->action_state_info_.montage_section_name = NAME_None;

	owner_->anim_instance_->UpdateAnimation();
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][#%d][Receive_SC_MONSTER_ACTION_END_CMD] Action mode: %s, Location: %s, Rotation: %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(action_mode), *location.ToString(), *rotation.ToString());
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type)
{
	switch (change_type)
	{
	case EMonsterBodyPartChangeType::Wound:
		{
			owner_->OnBodyPartWounded.Broadcast(body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Break:
		{
			owner_->OnBodyPartBroken.Broadcast(body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Sever:
		{
			owner_->OnBodyPartSevered.Broadcast(body_part);
			break;
		}
	}

	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][#%d][Receive_SC_MONSTER_PART_CHANGE_CMD] %s %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(body_part), *UAZUtility::EnumToString(change_type));
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_HIT_CMD(const FHitResultInfo hit_info)
{
	if (auto character = UGameplayStatics::GetPlayerCharacter(owner_->GetWorld(), 0))
	{
		AAZPlayer_Playable* player = Cast<AAZPlayer_Playable>(character);
		if (player)
		{
			owner_->OnHit.Broadcast(player, hit_info);
			UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][%d][Receive_SC_MONSTER_HIT_CMD] Damage dealt: %d, Position: %s"),
				owner_->object_serial_, hit_info.damage_amount, *hit_info.hit_location.ToString());
		}
	}
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][%d][Receive_SC_MONSTER_HIT_CMD] Damage dealt: %d, Position: %s"),
			owner_->object_serial_, hit_info.damage_amount, *hit_info.hit_location.ToString());
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_ENRAGE_BEGIN_CMD()
{
	owner_->OnEnraged.Broadcast();
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][%d][Receive_SC_MONSTER_ENRAGE_BEGIN_CMD]"), owner_->object_serial_);
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_ENRAGE_END_CMD()
{
	owner_->OnEnrageEnded.Broadcast();
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][%d][Receive_SC_MONSTER_ENRAGE_END_CMD]"), owner_->object_serial_);
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_DIE_CMD()
{
	owner_->OnDeath.Broadcast();
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent_Client][%d][Receive_SC_MONSTER_DIE_CMD]"), owner_->object_serial_);
}
