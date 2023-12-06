// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"

#include "Character/Monster/AZMonster.h"
#include "Character/Monster/AZMonster_Client.h"

void UAZAnimInstance_Monster::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAZAnimInstance_Monster::NativeUpdateAnimation(float delta_seconds)
{
	Super::NativeUpdateAnimation(delta_seconds);
}

void UAZAnimInstance_Monster::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	FString name;
	if (is_server_) name = GetMonster()->name_.ToString();
	else name = GetMonsterClient()->name_.ToString();
	FString flystart_anim_path = FString::Printf(TEXT("/Game/AZ/Monsters/%s/Animations/Locomotion/AM_%s_FlyBegin"), *name, *name);
	FString flyend_anim_path = FString::Printf(TEXT("/Game/AZ/Monsters/%s/Animations/Locomotion/AM_%s_FlyEnd"), *name, *name);
	fly_start_anim_ = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), this, *flystart_anim_path));
	fly_end_anim_ = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), this, *flyend_anim_path));
}

AAZMonster* UAZAnimInstance_Monster::GetMonster()
{
	if (owner_.Get())
	{
		return Cast<AAZMonster>(owner_);
	}
	return nullptr;
}

AAZMonster_Client* UAZAnimInstance_Monster::GetMonsterClient()
{
	if (owner_.Get())
	{
		return Cast<AAZMonster_Client>(owner_);
	}
	return nullptr;
}
