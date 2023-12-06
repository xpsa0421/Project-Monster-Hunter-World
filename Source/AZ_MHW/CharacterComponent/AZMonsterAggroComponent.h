// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZMonsterAggroComponent.generated.h"
enum class EMonsterBodyPartChangeType : uint8;
class AAZPlayer_Origin;
class AAZMonster;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZMonsterAggroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Initialisers
	UAZMonsterAggroComponent();
	void Init();
	void ActivateSystem();
	void InactivateSystem();
	void Reset();
	virtual void BeginPlay() override;
	
	// Setters
	void ForceSetBestTarget(AAZPlayer_Origin* character);
	void RemoveTarget(int32 player_serial);
	void UpdateBestTarget();
	
	// Getters
	int32 GetTargetSerial();
	AAZPlayer_Origin* GetTargetRef();
	FVector GetTargetLocation();
	FVector GetRandomTargetLocation();
	float GetDistance2DToTarget();
	float GetAngle2DToTarget();

	// Updaters
	void IncreaseByDamage(int32 attacker_serial, int32 damage);
	void IncreaseByPartChange(int32 attacker_serial, EMonsterBodyPartChangeType change_type);
	void ActivateByEnterCombat(int32 player_serial);
	
private:
	// Update aggro information
	void UpdateByRange();
	void UpdateAggroSpecific(int32 player_serial, int32 aggro_point, FString update_reason);
	
private:
	TObjectPtr<AAZMonster> owner_;

	int32 target_serial_;
	TObjectPtr<AAZPlayer_Origin> target_ref_;

	FTimerHandle inrange_update_timer_handle_;
	FTimerHandle target_update_timer_handle_;
	float inrange_update_rate_;
	float percept_range_;

	UPROPERTY() TMap<int32, int32> aggro_map_;
};