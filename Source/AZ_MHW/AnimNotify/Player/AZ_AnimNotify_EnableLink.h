// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZ_AnimNotify_EnableLink.generated.h"

/**
 * 현재 섹션이름을 가져가기
 */
UCLASS()
class AZ_MHW_API UAZ_AnimNotify_EnableLink : public UAnimNotify
{
	GENERATED_BODY()
#pragma region Inherited function
protected:
	/** */
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
#pragma endregion
public:
};
