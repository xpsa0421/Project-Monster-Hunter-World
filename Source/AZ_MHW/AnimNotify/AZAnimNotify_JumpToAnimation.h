// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_JumpToAnimation.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_JumpToAnimation : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAZAnimNotify_JumpToAnimation();
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString next_animation_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString next_montage_section_name;
};
