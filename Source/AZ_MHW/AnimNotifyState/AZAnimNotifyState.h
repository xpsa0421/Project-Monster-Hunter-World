// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimNotifies/AnimNotifyState.h>
#include "AZAnimNotifyState.generated.h"

/**
 * 애니메이션 피격면역 상태표현(기간버프)
 * 시작처리+끝남처리
 * 홀드, 캐스팅 같은
 * 지속적인 상태 확인
 * 
 * Montage_JumpToSection()함수를 호출해서 섹션을 넘어가도, 노티파이 엔드가 호출이 된다.
 */

UCLASS()
class AZ_MHW_API UAZAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAZAnimNotifyState();
#pragma region Inherited function
protected:
	/** */
	virtual void NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration, const FAnimNotifyEventReference& event_reference) override;
	/** */
	virtual void NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float frame_delta_time, const FAnimNotifyEventReference& event_reference) override;
	/** */
	virtual void NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
#pragma endregion
public:
	//UPROPERTY(EditAnywhere) FName NameID;
};
