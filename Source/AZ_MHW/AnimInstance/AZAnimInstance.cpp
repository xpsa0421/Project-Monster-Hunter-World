// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/Character/AZCharacter.h"

UAZAnimInstance::UAZAnimInstance()
{
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> EXAMPLE_MONTAGE(TEXT("/Game/"));
	//if(EXAMPLE_MONTAGE.Succeeded()) { //=EXAMPLE_MONTAGE.Object; }
}

void UAZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	owner_ = Cast<AAZCharacter>(TryGetPawnOwner());
}

void UAZAnimInstance::NativeUpdateAnimation(float delta_seconds)
{
	Super::NativeUpdateAnimation(delta_seconds);
	/** 이 함수는 다른 스레드에서 돌기때문에 객체 유효성 매업데이트마다 확인 해야함*/
	owner_ = Cast<AAZCharacter>(TryGetPawnOwner());
	
}
