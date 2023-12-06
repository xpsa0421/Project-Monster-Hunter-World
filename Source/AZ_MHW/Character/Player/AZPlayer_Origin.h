// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZPlayer.h"
#include "AZPlayer_Origin.generated.h"

/**
 * 서버에서 플레이되는 원본 캐릭터를 표현하는 객체
 * 해당 객체의 상태가 실제 유저 캐릭터 객체의 상태다.
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayer_Origin : public AAZPlayer
{
	GENERATED_BODY()

public:
	AAZPlayer_Origin();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void Tick(float delta_seconds) override;
	/** */
	virtual void BeginDestroy() override;
	/** */
	//virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component) override;
	/** */
	//virtual void PossessedBy(AController* new_controller) override;
#pragma endregion
#pragma region Animation Notify
public:
	UFUNCTION(BlueprintCallable) void SetActiveSuperArmor(bool enable, int32 reduce_rate);

	UFUNCTION() void AnimNotify_OnUseItem();

#pragma endregion 
#pragma region Damage Interface
public:
	virtual float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info) override;
protected:
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info) override;
	/** 다이나믹 델리게이트에서 처리되기 때문에 UFUNCTION 처리*/
	UFUNCTION() virtual void PostProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info);
#pragma endregion
};
