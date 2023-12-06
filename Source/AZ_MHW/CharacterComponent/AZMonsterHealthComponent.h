// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonsterHealthComponent.generated.h"

class AAZMonster;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZMonsterHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// Property Initialisers
	UAZMonsterHealthComponent();
	void Init();
	void InitializeRuntimeValues();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Property Getters
	float GetHealthRatio() const;
	float GetStaminaRatio() const;
	bool IsWounded(EMonsterBodyPart body_part);
	bool IsPendingKill() const;
	TMap<EMonsterBodyPart, FBossBodyPartState>* GetBodyPartStates();

	// Escape Functionalities
	//TODO bool CanEscape() const;

	// Damage Agent Overrides
	float ApplyDamage(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info);
	float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info);
	
protected:
	// Damage functions
	bool IsReceivedAttackValid(EDamageType damage_type, EMonsterBodyPart damaged_part);
	void ApplyDamageToBodyPart(EDamageType damage_type, EMonsterBodyPart damaged_part, float damage_amount);
	void CheckBeWounded(EMonsterBodyPart damaged_part, float damage_amount);
	void CheckBeBroken(EMonsterBodyPart damaged_part, float damage_amount);
	void CheckBeSevered(EMonsterBodyPart damaged_part, float damage_amount);
	void ReduceHealth(float amount);
	
	// Broadcasted via owner delegate
	UFUNCTION() void OnBodyPartWounded(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartWoundHealed(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartBroken(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartSevered(EMonsterBodyPart body_part);
	UFUNCTION() void OnEnraged();
	UFUNCTION() void OnEnrageEnded();
	UFUNCTION() void OnDeath();
	
private:
	TObjectPtr<AAZMonster> owner_;
	
	UPROPERTY(VisibleAnywhere, Category = "AZ | Health") int32 base_hp_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Stamina") int32 base_stamina_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Stamina") float tired_duration_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Health") FBossEscapeStats escape_stats_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") FBossWeaknessStats weakness_stats_;
	UPROPERTY(EditAnywhere, Category = "AZ | Damage") TMap<EMonsterBodyPart, FBossBodyPartState> body_part_states_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") float tenderised_damage_multiplier_;
	UPROPERTY(EditAnywhere, Category = "AZ | Damage") float wound_duration_;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Current State") int32 rage_accumulated_damage_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Current State") int32 current_hp_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Current State") int32 current_stamina_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Current State") int32 current_num_escapes_;
};
