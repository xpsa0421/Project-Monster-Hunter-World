// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "Components/Image.h"
#include "AZWidget_Launcher.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Launcher : public UAZWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void NativeTick(const FGeometry& my_geometry, float in_delta_time) override;
	virtual void OnAnimationStarted_Implementation(const UWidgetAnimation* animation) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* animation) override;

public:
	void OnBegin();

private:
	FTimerHandle OnBeginTimer;
};
