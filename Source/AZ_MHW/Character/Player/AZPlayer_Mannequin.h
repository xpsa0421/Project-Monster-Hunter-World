// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayer.h"
#include "AZPlayer_Mannequin.generated.h"

UCLASS()
class AZ_MHW_API AAZPlayer_Mannequin : public AAZPlayer
{
	GENERATED_BODY()

public:
	AAZPlayer_Mannequin();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	//virtual void Tick(float delta_seconds) override;
	/** */
	//virtual void BeginDestroy() override;
#pragma endregion
private:
	float angle = 0.f;
	bool rotate_mode = false;
public:
	void SetRotateMode(bool onoff);

	UPROPERTY(EditAnywhere) class USpringArmComponent* spring_arm_comp_;//camera arm
	UPROPERTY(EditAnywhere) class UCameraComponent* camera_comp_;//camera
};
