// Copyright Team AZ. All Rights Reserved.

#include "AZMonsterPacketHandlerComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Character/Player/AZPlayer_Origin.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"


// Sets default values for this component's properties
UAZMonsterPacketHandlerComponent::UAZMonsterPacketHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAZMonsterPacketHandlerComponent::Init()
{
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterPacketHandlerComponent] Invalid owner actor!"));
	}

	game_instance_ = Cast<UAZGameInstance>(GetOwner()->GetWorld()->GetGameInstance());
}

void UAZMonsterPacketHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	// bind delegate functions
	owner_->OnBodyPartWounded.AddUObject(this, &UAZMonsterPacketHandlerComponent::OnBodyPartWounded);
	owner_->OnBodyPartBroken.AddUObject(this, &UAZMonsterPacketHandlerComponent::OnBodyPartBroken);
	owner_->OnBodyPartSevered.AddUObject(this, &UAZMonsterPacketHandlerComponent::OnBodyPartSevered);
	owner_->OnEnterCombat.AddDynamic(this, &UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ENTER_COMBAT_CMD);
	owner_->OnEnraged.AddDynamic(this, &UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ENRAGE_BEGIN_CMD);
	owner_->OnEnrageEnded.AddDynamic(this, &UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ENRAGE_END_CMD);
	owner_->OnHit.AddDynamic(this, &UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_HIT_CMD);
	owner_->OnDeath.AddDynamic(this, &UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_DIE_CMD);
}

// 플레이어 인게임 조인 패킷이 들어왔을때 몬스터를 스폰한 이후 호출되며 새로 들어온 클라이언트에 몬스터를 배치할 것을 명령한다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_SPAWN_CMD(UINT32 newly_joined_client_idx)
{
	SC_MONSTER_SPAWN_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.monster_id = owner_->monster_id_;
	packet.rank = owner_->rank_;
	packet.location = owner_->GetActorLocation();
	packet.rotation = owner_->GetActorRotation();
	
	game_instance_->SendPacketFunc(newly_joined_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_SPAWN_CMD] ID: %d, Location: %s, Rotation %s"),
		owner_->object_serial_, packet.monster_id, *packet.location.ToString(), *packet.rotation.ToString());
}

// 플레이어의 인게임 로딩이 끝난 뒤 몬스터의 최종 트랜스폼 설정을 명령할 때 새로 접속한 플레이어에게 전송된다 (is_forced = true)
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_TRANSFORM_CMD(bool is_forced, UINT32 newly_joined_client_idx)
{
	SC_MONSTER_TRANSFORM_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.location = owner_->GetActorLocation();
	packet.rotation = owner_->GetActorRotation();
	packet.is_forced = is_forced;

	if (newly_joined_client_idx == 0)
		game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	else
		game_instance_->SendPacketFunc(newly_joined_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));
	
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_TRANSFORM_CMD] %s, %s, Location: %s, Rotation %s"),
		owner_->object_serial_, newly_joined_client_idx == 0 ? TEXT("BROADCASTED") : TEXT(""), is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *packet.location.ToString(), *packet.rotation.ToString());
}

// 플레이어의 인게임 로딩이 끝난 뒤 몬스터의 최종 메시 상태(상처, 부위파괴 등)의 설정을 위해 새로 접속한 플레이어에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_BODY_STATE_CMD(UINT32 newly_joined_client_idx)
{
	TMap<EMonsterBodyPart, FBossBodyPartState>* states = owner_->health_component_->GetBodyPartStates();

	SC_MONSTER_BODY_STATE_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.head_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Head));
	packet.back_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Back));
	packet.left_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::LeftWing));
	packet.right_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::RightWing));
	packet.tail_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Tail));

	game_instance_->SendPacketFunc(newly_joined_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_BODY_STATE_CMD]"), owner_->object_serial_);
}

// 몬스터가 전투모드에 돌입할 때 노래 재생/UI등 업데이트를 위해 인게임 접속자 모두에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ENTER_COMBAT_CMD()
{
	SC_MONSTER_ENTER_COMBAT_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_ENTER_COMBAT_CMD);
	packet.packet_length = sizeof(packet);
	
	game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_ENTER_COMBAT_CMD]"), owner_->object_serial_);
}

// 1. 몬스터의 액션 상태가 변경될 때 인게임 접속자 모두에게 전송된다
// 2. 플레이어의 첫 인게임 접속 시 현재 진행중인 액션 정보를 해당 플레이어에게 전송한다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ACTION_START_CMD(float start_position, UINT32 newly_joined_client_idx)
{
	SC_MONSTER_ACTION_START_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.state_info = MonsterActionStateInfo(owner_->action_state_info_);
	packet.start_position = start_position;

	if (newly_joined_client_idx == 0) // no client defined
		game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	else
		game_instance_->SendPacketFunc(newly_joined_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));

	UE_LOG(AZMonster_Network, Log, TEXT("\n[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_ACTION_START_CMD] %s, %s, %s, Action name: %s, Montage section: %s"),
		owner_->object_serial_, newly_joined_client_idx == 0 ? TEXT("BROADCASTED") : TEXT(""),
		*UAZUtility::EnumToString(owner_->action_state_info_.priority_score),
		*UAZUtility::EnumToString(owner_->action_state_info_.move_state),
		*owner_->action_state_info_.animation_name.ToString(),
		*owner_->action_state_info_.montage_section_name.ToString());
}

// 몬스터의 액션이 끝난 뒤 동기화를 위해 인게임 접속자 모두에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ACTION_END_CMD()
{
	SC_MONSTER_ACTION_END_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.location = owner_->GetActorLocation();
	packet.rotation = owner_->GetActorRotation();
	packet.action_mode = owner_->action_state_info_.action_mode;

	game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_ACTION_END_CMD] ActionMode: %s, Location: %s, Rotation %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(packet.action_mode), *packet.location.ToString(), *packet.rotation.ToString());
}

// 상처, 부위파괴, 절단 등 메시에 변화가 일어났을 때 인게임 접속자 모두에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type)
{
	SC_MONSTER_PART_CHANGE_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.body_part = body_part;
	packet.change_type = change_type;

	game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_PART_CHANGE_CMD], %s %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(body_part), *UAZUtility::EnumToString(change_type));
}

// 플레이어의 공격이 몬스터에 맞았을 때 UI, 이펙트를 업데이트 하기 위해 공격한 플레이어에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_HIT_CMD(AAZPlayer_Origin* attacker, FHitResultInfo hit_info)
{
	SC_MONSTER_HIT_CMD packet;
	packet.object_serial = owner_->object_serial_;
	packet.hit_info = hit_info;

	// retrieve attacker client index
	UINT32 attacker_client_idx = attacker->player_character_state_->uid_;
	game_instance_->SendPacketFunc(attacker_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_HIT_CMD], From: %d, Damage dealt: %d, Position: %s"),
		owner_->object_serial_, attacker_client_idx, hit_info.damage_amount, *hit_info.hit_location.ToString());
}

// 분노상태 돌입
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ENRAGE_BEGIN_CMD()
{
	SC_MONSTER_ENRAGE_BEGIN_CMD packet;
	packet.object_serial = owner_->object_serial_;
	game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_ENRAGE_BEGIN_CMD]"), owner_->object_serial_);
}

// 분노상태 해제
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ENRAGE_END_CMD()
{
	SC_MONSTER_ENRAGE_END_CMD packet;
	packet.object_serial = owner_->object_serial_;
	game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_ENRAGE_END_CMD]"), owner_->object_serial_);
}

// 몬스터가 사망했을 때 노래/UI등의 업데이트를 위해 모든 플레이어에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_DIE_CMD()
{
	SC_MONSTER_DIE_CMD packet;
	packet.object_serial = owner_->object_serial_;
	game_instance_->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_DIE_CMD]"), owner_->object_serial_);
}

// 클라이언트에서 몬스터 스폰에 성공했을 때 서버에게 몬스터 상태의 업데이트를 요청한다
void UAZMonsterPacketHandlerComponent::Receive_CS_MONSTER_UPDATE_REQ(UINT32 client_index)
{
	Send_SC_MONSTER_ACTION_START_CMD(owner_->anim_instance_->GetSkelMeshComponent()->GetPosition(), client_index);
	Send_SC_MONSTER_TRANSFORM_CMD(true, client_index);
	Send_SC_MONSTER_BODY_STATE_CMD(client_index);
}

void UAZMonsterPacketHandlerComponent::OnBodyPartWounded(EMonsterBodyPart body_part)
{
	Send_SC_MONSTER_PART_CHANGE_CMD(body_part, EMonsterBodyPartChangeType::Wound);
}

void UAZMonsterPacketHandlerComponent::OnBodyPartBroken(EMonsterBodyPart body_part)
{
	Send_SC_MONSTER_PART_CHANGE_CMD(body_part, EMonsterBodyPartChangeType::Break);
}

void UAZMonsterPacketHandlerComponent::OnBodyPartSevered(EMonsterBodyPart body_part)
{
	Send_SC_MONSTER_PART_CHANGE_CMD(body_part, EMonsterBodyPartChangeType::Sever);
}
