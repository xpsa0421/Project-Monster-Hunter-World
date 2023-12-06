// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimInstance.h>
#include "AZAnimInstance.generated.h"

class AAZCharacter;

/**
 * ABP 부모클래스의 Base Class
 * AnimInstance는 AnimClass + AnimGraph의 구조를 이루며,
 * AnimClass는 AnimGraph가 사용할 데이터와, 애니메이션 콜백을 정의
 * 
 * 모든 일반 틱 업데이트 >> 애님 업데이트가 호출된 후에 애님 그래프가 실행된다.
 * AnimGraph(BluePrint로만 작성이 가능한 애니메이션 그래프.)
 * (스레드가 분리되어 있기 때문에 스레드 안전을 위해 변수를 참조만 가능하다. 변경하기 위해서는 애니메이션의 노티파이에 심어서 이벤트콜백으로 해야한다.)
 * 애니메이션(최종포즈)을 한다.
 */
UCLASS(abstract)
class AZ_MHW_API UAZAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAZAnimInstance();
	
#pragma region Inherited function
protected:
	/**
	 * 애님 인스턴스 애니메이션 초기화
	 * 애니메이션 실행 조건값 초기화용도
	 */
	virtual void NativeInitializeAnimation() override;
	/**
	 * 애님 인스턴스 캡쳐 업데이트
	 * 애니메이션 실행 조건값 동기화용도
	 */
	virtual void NativeUpdateAnimation(float delta_seconds) override;
	
#pragma endregion
public:
	/** 일반 AnimNotify 클래스는 노티파이명(NotifyName)으로 된 이 함수를 이름으로 다이나믹 델리게이트로 호출한다.
	 */
	//void AnimNotify_NotifyName();
	
	UPROPERTY(BlueprintReadOnly, Category = AZ)
	TObjectPtr<AAZCharacter> owner_;
};