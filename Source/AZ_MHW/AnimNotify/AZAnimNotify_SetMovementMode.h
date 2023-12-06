// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include <Containers/EnumAsByte.h>
#include "AZAnimNotify_SetMovementMode.generated.h"

UCLASS()
class AZ_MHW_API UAZAnimNotify_SetMovementMode : public UAnimNotify
{
	GENERATED_BODY()
	UAZAnimNotify_SetMovementMode();

public:
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TEnumAsByte<EMovementMode> movement_mode_;
};
