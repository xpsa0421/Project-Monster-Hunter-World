// Copyright Team AZ. All Rights Reserved.


#include "AZ_AnimNotify_DisableLink.h"

#include "AnimInstance/AZAnimInstance_Player.h"

void UAZ_AnimNotify_DisableLink::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if(const auto anim_instance = Cast<UAZAnimInstance_Player>(mesh_comp->GetAnimInstance()))
	{
		anim_instance->can_input_control_ = false;
	}
}
