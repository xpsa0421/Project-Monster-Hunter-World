// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/AZCharacter.h"
#include "AZMonster_Model.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZMonster_Model : public AAZCharacter
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	void BlinkEyes();
	void CloseEyes(bool should_close);
	void SetMaterialVisibility(bool is_visible);

	TObjectPtr<UMaterialInterface> eyelid_material_;
	bool eyes_closed_;
	FTimerHandle blink_eye_timer_handle_;
};
