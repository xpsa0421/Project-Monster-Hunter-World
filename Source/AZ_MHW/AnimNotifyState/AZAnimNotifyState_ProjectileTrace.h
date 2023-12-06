// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZAnimNotifyState_ProjectileTrace.generated.h"

/**
 * 플레이어 애니메이션용
 * 플레이어가 조준사격하는 애니메이션을 하면
 * 발생하는 궤적을 그리는 노티파이스테이트
 */
UCLASS()
class AZ_MHW_API UAZAnimNotifyState_ProjectileTrace : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	/** */
	UAZAnimNotifyState_ProjectileTrace();
	
	/** */
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	/** */
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	/** */
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	//캐릭터 방향(총구), 각도, 세기
	//중력계수
};
