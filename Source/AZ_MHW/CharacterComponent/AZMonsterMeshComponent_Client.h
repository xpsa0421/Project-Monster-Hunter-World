// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonsterMeshComponent_Client.generated.h"

class AAZMonster_Client;
enum class EMonsterBodyPart : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZMonsterMeshComponent_Client : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAZMonsterMeshComponent_Client();
	
	void Init();
	virtual void BeginPlay() override;
	void SetBodyState(EMonsterBodyPart body_part, FBossBodyPartDebuffState state);

protected:
	// Property Initialisers
	void SetUpDynamicMaterials();
	void SetUpBodyPartMaterialMaps();
	void InitializeMeshVisibilities();
	
	// Body Mesh Material Opacity Setters
	void SetMaterialVisibility(uint8 material_idx, bool is_visible);
	void SetMaterialVisibility(FName slot_name, bool is_visible);

	// Responders to body state changes
	UFUNCTION() void OnBodyPartWounded(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartWoundHealed(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartBroken(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartSevered(EMonsterBodyPart body_part);
	UFUNCTION() void OnDeath();

	UFUNCTION() void CloseEyes(bool should_close);
	UFUNCTION() void BlinkEyes();
	
private:
	TObjectPtr<AAZMonster_Client> owner_;
	TObjectPtr<USkeletalMeshComponent> mesh_;
	UPROPERTY(VisibleAnywhere) TMap<EMonsterBodyPart, bool> wound_map_;

	// material maps
	UPROPERTY(VisibleAnywhere, Category = "AZ | Material")
	TMap<EMonsterBodyPart, int32> mesh_material_indices_default_;

	UPROPERTY(VisibleAnywhere, Category = "AZ | Material")
	TMap<EMonsterBodyPart, int32> mesh_material_indices_wounded_;

	UPROPERTY(VisibleAnywhere, Category = "AZ | Material")
	TMap<EMonsterBodyPart, int32> mesh_material_indices_cutsurface_;

	// eye blink
	UPROPERTY(VisibleAnywhere) bool eyes_closed_;
	FTimerHandle blink_eye_timer_handle_;
};
