// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZAnimNotifyState_ShapeTrace.generated.h"

/**
 * 플레이어 애니메이션용
 *
 * 특정 모양으로 범위 스윕 체크, 장판기(설치) 혹은 볼륨이 있는 타격
 */
UCLASS()
class AZ_MHW_API UAZAnimNotifyState_ShapeTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAZAnimNotifyState_ShapeTrace();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
