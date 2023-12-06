#include "AZ_MHW/AnimNotify/AZAnimNotify_SetMovementMode.h"
#include "AZ_MHW/Character/AZCharacter.h"

UAZAnimNotify_SetMovementMode::UAZAnimNotify_SetMovementMode()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
#endif
}

void UAZAnimNotify_SetMovementMode::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if (mesh_comp == nullptr || mesh_comp->GetOwner() == nullptr)
		return;

	if (mesh_comp->GetOwner()->IsA(AAZCharacter::StaticClass()))
	{
		AAZCharacter* character = Cast<AAZCharacter>(mesh_comp->GetOwner());
		if (character)
		{
			character->AnimNotify_SetMovementMode(movement_mode_);
		}
	}
}