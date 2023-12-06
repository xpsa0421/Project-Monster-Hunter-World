// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_EndOfAction.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_EndOfAction : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAZAnimNotify_EndOfAction();

	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
};
