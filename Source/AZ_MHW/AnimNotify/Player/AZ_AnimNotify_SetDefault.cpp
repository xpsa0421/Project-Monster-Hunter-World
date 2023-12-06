// Copyright Team AZ. All Rights Reserved.


#include "AZ_AnimNotify_SetDefault.h"
#include "AnimInstance/AZAnimInstance_Player.h"

void UAZ_AnimNotify_SetDefault::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if(const auto anim_instance = Cast<UAZAnimInstance_Player>(mesh_comp->GetAnimInstance()))
	{
		anim_instance->current_section_name_ = TEXT("Default");
		anim_instance->can_input_control_ = true;
	}
}
