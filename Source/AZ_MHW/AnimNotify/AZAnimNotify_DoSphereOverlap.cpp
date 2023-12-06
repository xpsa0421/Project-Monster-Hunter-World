// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimNotify/AZAnimNotify_DoSphereOverlap.h"
#include "AZ_MHW/Character/AZCharacter.h"

void UAZAnimNotify_DoSphereOverlap::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if (mesh_comp == nullptr || mesh_comp->GetOwner() == nullptr)
		return;

	if (mesh_comp->GetOwner()->IsA(AAZCharacter::StaticClass()))
	{
		AAZCharacter* character = Cast<AAZCharacter>(mesh_comp->GetOwner());
		if (character)
		{
			character->AnimNotify_DoSphereOverlap(socket_name_, radius_);
		}
	}
}
