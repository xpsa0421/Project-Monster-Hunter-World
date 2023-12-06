#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW/Environment/AZActor.h"
#include <TimerManager.h>
#include "AZActor_Interaction.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AZ_MHW_API AAZActor_Interaction : public AAZActor
{
	GENERATED_BODY()

public:
	AAZActor_Interaction();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ Loading")
	class USceneCaptureComponent2D* scene_capture_component;

	void StopCapture();
};