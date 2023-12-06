
#include "AZ_MHW/AnimNotify/AZAnimNotify.h"

UAZAnimNotify::UAZAnimNotify()
{
	
}

void UAZAnimNotify::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);
	
}
