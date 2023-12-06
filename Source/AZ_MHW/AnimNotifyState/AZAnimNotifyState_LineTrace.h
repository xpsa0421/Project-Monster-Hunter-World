// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZAnimNotifyState_LineTrace.generated.h"

class AAZSocketActor;

/**
 * 플레이어 애니메이션용
 *  Player의 무기 타입을 알아내고, 소켓액터을 찾아서
 *  무기길이 만큼 선을 매틱마다 쏴서, 피격체크를 하는 노티파이입니다.
 *  LineTrace을 매틱마다 쏴서, 칼의 궤적 만든다.
 *
 *  SphereTrace를 매틱마다 체크한다면?
 */
UCLASS()
class AZ_MHW_API UAZAnimNotifyState_LineTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAZAnimNotifyState_LineTrace();
	
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
	/** 단일 타겟인지? 아니면 피격된 모든 타겟에 전달*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool is_single_target_;
	
	/** 단일 타격인지? 아니면 지속적으로 일정시간마다 지속 타격*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool is_hit_once_;
	
	/** 만약 다단 히트일시, 해당 애니메이션의 몇틱 주기로 데미지를 전달할 것인지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float hit_tick_second_;
	
	/** */
	UPROPERTY(EditAnywhere)
	FName main_socket_actor_name_;
	/** */
	UPROPERTY(EditAnywhere)
	FName sub_socket_actor_name_;
	
	
	//다단타격에 모든 타겟에 전달해야한다면?
	
	//float length//무기타입별 트레이스 길이

	//위치와 각도를 구하기 위해서 actor를 소유해야한다.
	UPROPERTY(BlueprintReadWrite)
	AAZSocketActor* main_socket_actor_;

	/** 쌍검이라면?*/
	UPROPERTY(BlueprintReadWrite)
	AAZSocketActor* sub_socket_actor_;

	UPROPERTY(BlueprintReadWrite)
	bool is_already;

	//TEMP
	UFUNCTION(BlueprintCallable)
	void SetDisable();
	
private:
	/** 맞았던 액터들(순서는 상관없기 때문에 맞았는지 안맞았는지 체크만 하기위해 TSet을 사용한다.)
	 * 
	 */
	TSet<AActor*> got_hit_actors_;//틱마다 초기화할것인지
	
	/** 맞았던 액터인데 다시 맞을 수 있는 액터들
	 * 이터레이터로 빼서 시간 검색
	 * 
	 */
	//TMap<float, AActor*> got_hit_actors_;
};
