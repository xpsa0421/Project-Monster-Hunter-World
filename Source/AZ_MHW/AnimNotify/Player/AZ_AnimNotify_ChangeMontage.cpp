// Copyright Team AZ. All Rights Reserved.


#include "AZ_AnimNotify_ChangeMontage.h"
#include "AnimInstance/AZAnimInstance_Player.h"

void UAZ_AnimNotify_ChangeMontage::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if(const auto& anim_instance = mesh_comp->GetAnimInstance())
	{
		if(const auto& player_anim_instance = Cast<UAZAnimInstance_Player>(anim_instance))
		{
			player_anim_instance->is_montage_ = true;
			player_anim_instance->should_transition_ = true;
			player_anim_instance->next_montage_name_ = target_montage_name_;
			player_anim_instance->next_section_name_ = target_section_name_;
		}
	}
}
