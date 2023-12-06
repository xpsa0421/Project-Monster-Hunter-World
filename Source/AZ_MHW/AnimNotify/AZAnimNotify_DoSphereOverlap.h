// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_DoSphereOverlap.generated.h"

enum class EEffectDurationType : uint8;

UCLASS()
class AZ_MHW_API UAZAnimNotify_DoSphereOverlap : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName socket_name_;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float radius_;
};
