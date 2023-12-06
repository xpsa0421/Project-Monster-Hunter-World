// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimNotifies/AnimNotify.h>
#include "AZAnimNotify.generated.h"

#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration

#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion

/**
 * 애니메이션 특정시점에 트리거되는 알림 클래스
 * 조건 처리(키입력, 상태체크)
 * 즉시 처리
 *
 * 
 * Montage_JumpToSection()함수를 호출하여 섹션을 넘어갈 경우, 노티파이가 호출되지 않는다.
 */

UCLASS()
class AZ_MHW_API UAZAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAZAnimNotify();
#pragma region Inherited function
protected:
	/** */
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
#pragma endregion
public:
	//UPROPERTY(EditAnywhere) FName NameID;
};

