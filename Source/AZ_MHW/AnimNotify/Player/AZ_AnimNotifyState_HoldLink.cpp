// Copyright Team AZ. All Rights Reserved.


#include "AZ_AnimNotifyState_HoldLink.h"
#include "AnimInstance/AZAnimInstance_Player.h"
#include "Character/Player/AZPlayer.h"
#include "PlayerState/AZPlayerState_Client.h"

void UAZ_AnimNotifyState_HoldLink::NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
                                               float total_duration, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyBegin(mesh_comp, animation, total_duration, event_reference);
	is_next_link_ = false;
	if(const auto& anim_instance = Cast<UAZAnimInstance_Player>(mesh_comp->GetAnimClass()))
	{
		owner_anim_instance_ = anim_instance;
	}
	if(const auto& player = Cast<AAZPlayer>(mesh_comp->GetOwner()))
	{
		owner_ = player;
	}
}

void UAZ_AnimNotifyState_HoldLink::NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	float frame_delta_time, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyTick(mesh_comp, animation, frame_delta_time, event_reference);
	
	if((owner_->player_character_state_->action_state_.input_bitmask ^ input_bitmask_) == 0 || !is_next_link_)
	{
		//is_next_link_ = false;
	}
	else
	{
		is_next_link_ = true;
	}
}

void UAZ_AnimNotifyState_HoldLink::NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyEnd(mesh_comp, animation, event_reference);

	if(is_next_link_)
	{
		owner_anim_instance_->next_montage_name_ = target_montage_name_;
		owner_anim_instance_->next_section_name_ = target_section_name_;
		owner_anim_instance_->should_transition_ = true;
	}
}
