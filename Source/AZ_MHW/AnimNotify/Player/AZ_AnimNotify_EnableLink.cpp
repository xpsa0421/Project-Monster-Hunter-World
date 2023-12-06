// Copyright Team AZ. All Rights Reserved.


#include "AZ_AnimNotify_EnableLink.h"
#include "AnimInstance/AZAnimInstance_Player.h"

void UAZ_AnimNotify_EnableLink::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if(const auto anim_instance = Cast<UAZAnimInstance_Player>(mesh_comp->GetAnimInstance()))
	{
		anim_instance->current_section_name_ = anim_instance->Montage_GetCurrentSection(anim_instance->current_anim_montage_);
		anim_instance->can_input_control_ = true;
	}
}
