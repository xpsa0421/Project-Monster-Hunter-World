// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZAnimInstance_Monster.generated.h"

class AAZMonster;
class AAZMonster_Client;
class UBlendSpace1D;

UCLASS()
class AZ_MHW_API UAZAnimInstance_Monster : public UAZAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float delta_seconds) override;
	virtual void NativeBeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent) void UpdateAnimation();
	UFUNCTION(BlueprintCallable) AAZMonster* GetMonster();
	UFUNCTION(BlueprintCallable) AAZMonster_Client* GetMonsterClient();
	UFUNCTION(BlueprintImplementableEvent) float GetCurrentPosition();
		
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AZ") bool is_server_;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AZ") TWeakObjectPtr<AAZMonster> owner_monster_;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AZ") TWeakObjectPtr<AAZMonster_Client> owner_monster_client_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Animation") bool is_doing_action_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Animation") float start_position_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Animation") float current_position_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Animation") bool is_playing_montage_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Animation") bool is_playing_blendspace_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Animation") UAnimMontage* fly_start_anim_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Animation") UAnimMontage* fly_end_anim_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Animation") UAnimMontage* death_montage_;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZ | Animation") TMap<FName, UBlendSpace1D*> blendspaces_;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZ | Animation") TMap<FName, UAnimMontage*> montages_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Animation") UBlendSpace1D* active_blendspace_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | Animation")	float current_play_rate_;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | State") FMonsterActionStateInfo active_action_state_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") FMonsterActionStateInfo new_action_state_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") bool is_low_stamina_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") bool is_rage_mode_;
};

/*
 *	TODO:
 *	1. How to handle actions that require damage check?
 *		notify? table? how do i know which action to play? by id?
 *		percentage of action failure also required in the table
 *
 *	2. Check for rage -> when rage mode entered, fire an event
 *		in the animation instance to set the boolean value
 *
 *	3. Do the same for low stamina
*/