// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZ_AnimNotify_SetDefault.generated.h"

/**
 * 섹션의 종료마다 종료알림 노티파이를 심어서 사용하지 않고,
 * 몽타주 기본 섹션 연결기능을 사용한 후
 * 디폴트로 돌아오면 종료 갱신해주는 기능
 */
UCLASS()
class AZ_MHW_API UAZ_AnimNotify_SetDefault : public UAnimNotify
{
	GENERATED_BODY()

#pragma region Inherited function
protected:
	/** */
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
#pragma endregion
public:
};
