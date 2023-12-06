﻿// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZ_AnimNotify_ChangeMontage.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZ_AnimNotify_ChangeMontage : public UAnimNotify
{
	GENERATED_BODY()
#pragma region Inherited function
protected:
	/** */
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
#pragma endregion
public:
	UPROPERTY(EditAnywhere)
	FName target_montage_name_;
	UPROPERTY(EditAnywhere)
	FName target_section_name_;
};
