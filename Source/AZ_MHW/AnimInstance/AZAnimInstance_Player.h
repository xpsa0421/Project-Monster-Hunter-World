// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZAnimInstance_Player.generated.h"

//TODO 무기도 애님인스턴스클래스 있어야함.

DECLARE_DELEGATE_RetVal_OneParam(bool, FTriggerEndAnimation, UAZAnimInstance*)//애니메이션 종료조건 트리거 델리게이트

class AAZPlayer;
class AAZPlayerState_Client;

/**
 * 플레이어 애니메이션블루프린트의 뼈대
 * 플레이어 캐릭터의 실제 움직임만을 관리하는 역할.(회전(보간+애니메이션), 이동(애니메이션))
 * Controller(Input)와 Character(Event)로 변경된 State(Data)를 캐싱
 * Data를 해석해 해당 틱에 실행할 AnimGraph를 실행
 * 이 클래스는 순수하게 자기정보로만 돌아가야 한다.
 */
UCLASS(Abstract)
class AZ_MHW_API UAZAnimInstance_Player : public UAZAnimInstance
{
	GENERATED_BODY()

public:
	UAZAnimInstance_Player();

#pragma region Inherited function
protected:
	/** */
	virtual void NativeInitializeAnimation() override;
	/** */
	virtual void NativeUpdateAnimation(float delta_seconds) override;
#pragma endregion
private:
	
	/** 블루 프린트(AnimGraph)용 AnimInstance 변수들
	 * 결과적으로 해당 변수들을 컨트롤 하는 것으로 애니메이션을 모두 컨트롤 할 수 있다.
	 * 모든 클라이언트에서 캐릭터는 시작위치, 시작방향, 시작시간, 재생속도, 실행 애니메이션이 동일하면 동일한 결과를 가지도록 한다.
	 * 서버로부터 시작위치,방향조정된후 애니메이션설정을 받고 실행명령을 받으면 매 업데이트 실행되도록 한다.
	 */

	/** 애니메이션 전환 명령 트리거처리 */
	void OnAnimationTransitionTrigger();
	
	//애니메이션 실행 조건
	//강제 액션: 바로 실행 (should_transition으로 실행)
	//지연 액션: 애니메이션 종료시 실행(Section의 구분마다 AnimNotify_EndTrigger)
	//조건 액션: 예를 들면 키입력 중단등
	//--델리게이트: True False를 반환하는 조건체크 함수를 델리게이트에 바인딩하고, 업데이트에서 갱신하면서 조건 체크
	//--노티파이: 이 클래스의 변수를 변경하는 노티파이를 애니메이션에 바인딩하고, 애니메이션에서 갱신하면서 조건 체크

	//애니메이션 회전보간(노티파이로 기능)
	//--입력회전보간필요: 입력한 방향 값에 대한 보간처리만 있음.(이동, 턴)
	//--입력회전보간없음: 애니메이션의 루트 회전처리만 있음.
public:
	UPROPERTY(BlueprintReadWrite, Category="AZ|Animation") bool is_montage_;//현재 애니메이션이 몽타주 모드인지
	UPROPERTY(BlueprintReadWrite, Category="AZ|Animation") bool should_transition_;//현재 애니메이션을 전환 해야하는지
	UPROPERTY(BlueprintReadWrite, Category="AZ|Animation") bool is_rotation_;//현재 애니메이션이 강제 회전보간(목표 방향으로)인지
	UPROPERTY(BlueprintReadWrite, Category="AZ|Animation") bool can_input_control_;//현재 애니메이션을 입력으로 조종이 가능한지

	UPROPERTY(BlueprintReadWrite, Category="AZ|Animation") int32 anim_state_bitmask_;//애니메이션 상태 비트마스크
	UPROPERTY(BlueprintReadWrite, Category="AZ|Animation") int32 anim_command_bitmask_;//애니메이션 커맨드 비트마스크
	
	/** 매 업데이트마다 애니메이션 종료여부를 반환하는 델리게이트를 검사합니다.*/
	FTriggerEndAnimation handle_animation_transition_trigger_;//애니메이션 전환 트리거 함수지정
	
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FRotator current_forward_direction_; //캐릭터의 전면 방향
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FRotator next_forward_direction_; //최종 전면 방향
	
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") UAnimSequence* current_anim_sequence_;//현재 애님 시퀀스
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") UAnimMontage* current_anim_montage_;//현재 애님 몽타주

	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") float current_anim_play_rate_;//현재 애님 재생속도

	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FName current_sequence_name_;//현재 시퀀스 이름
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FName next_sequence_name_;//다음 시퀀스 이름
	
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FName current_montage_name_;//현재 몽타주 이름
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FName current_section_name_;//현재 섹션 이름

	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FName next_montage_name_;//다음 몽타주 이름
	UPROPERTY(BlueprintReadOnly, Category="AZ|Animation") FName next_section_name_;//다음 섹션 이름

	//즉시 지정 애니메이션으로
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void SetDirectAnimSequence(FName sequence_name = NAME_None);
	//즉시 지정 몽타주로
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void SetDirectAnimMontage(FName montage_name = NAME_None);
	//즉시 지정 섹션으로
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void SetDirectSection(FName section_name = TEXT("Default"));
	
	/** 지연(애니메이션 시퀀스 종료) 다음 시퀀스로 //사용안함. 폐기*/
	//UFUNCTION(BlueprintCallable,Category="AZAnimSystem")
	//void SetNextAnimSequence(FName sequence_name = NAME_None);
	/** 지연(애니메이션 시퀀스 종료) 다음 몽타주로*/
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void SetNextAnimMontage(FName montage_name = NAME_None);
	/** 지연(애니메이션 시퀀스 종료) 다음 섹션으로*/
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void SetNextSection(FName section_name = TEXT("Default"));

	/** 애니메이션을 일시정지합니다.*/
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void PauseAnimation();
	/** 애니메이션을 재개합니다.*/
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void ResumeAnimation();
	/** 애니메이션 실행 속도를 조정합니다. 캐릭터 스피드를 표현합니다.*/
	UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	void SetAnimPlayRate(int32 play_rate = 20000);


	UFUNCTION()
	void AnimNotify_OnUseItem();
	
	// /** 테이블에서 지정한 액션키 값에 해당하는 플레이어 애니메이션값으로 수행합니다.*/
	// UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	// void SetAnimationFromTable(int32 anim_code);
	// UFUNCTION(BlueprintCallable, Category="AZ|Animation")
	// void SetSequenceFromTable(int32 anim_code);


	//테이블 못만들고, 블루프린트로 타협하기
	UPROPERTY(BlueprintReadWrite)
	float input_direction_;
	UPROPERTY(BlueprintReadWrite)
	int32 input_bitmask_;
	
	UFUNCTION(BlueprintCallable)
	bool CorrectInputBitMask(int32 bitmask) const;
	
	UFUNCTION(BlueprintCallable)
	bool CheckInputBitMask(int32 bitmask) const;

	UFUNCTION(BlueprintCallable)
	bool CheckMontage(FName montage_name) const;

	UFUNCTION(BlueprintCallable)
	bool CheckSection(FName section_name) const;

	UFUNCTION(BlueprintCallable)
	void SetAnimationProperty(bool is_rotation, bool can_control);

	UFUNCTION(BlueprintCallable)
	void RecoverStamina(int recover_stamina);
	
	UFUNCTION(BlueprintCallable)
	bool CheckCostAnimation();
	
	UFUNCTION(BlueprintCallable)
	void ForceImmediatelyRotate();
};