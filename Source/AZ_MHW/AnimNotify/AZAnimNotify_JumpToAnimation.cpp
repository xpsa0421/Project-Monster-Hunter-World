// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimNotify/AZAnimNotify_JumpToAnimation.h"
#include "AZ_MHW/Character/AZCharacter.h"

UAZAnimNotify_JumpToAnimation::UAZAnimNotify_JumpToAnimation()
{
#if WITH_EDITORONLY_DATA
  NotifyColor = FColor::Purple;
#endif
}

void UAZAnimNotify_JumpToAnimation::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
  Super::Notify(mesh_comp, animation, event_reference);

  if (mesh_comp == nullptr || mesh_comp->GetOwner() == nullptr)
    return;

  if (mesh_comp->GetOwner()->IsA(AAZCharacter::StaticClass()))
  {
    AAZCharacter* character = Cast<AAZCharacter>(mesh_comp->GetOwner());
    if (character)
    {
      character->AnimNotify_JumpToAnimation(next_animation_name, next_montage_section_name);
    }
  }
}