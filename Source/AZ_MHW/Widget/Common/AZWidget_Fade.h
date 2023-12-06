// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "Components/Image.h"
#include "AZWidget_Fade.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Fade : public UAZWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void ForceHide() override {} // 미사용
	virtual void RestoreFromForceHidden() override {} // 미사용
	virtual void NativeTick(const FGeometry& my_geometry, float in_delta_time) override;

protected:
	void Fade(const float duration, const bool in_to_black);

public:
	void FadeIn(const float fade_time, bool force = false);
	void FadeOut(const float fade_time, bool force = false);
	void FadeInOut(const float fade_in_time, const float fade_out_time);

	DECLARE_EVENT(UAZGameViewportClient, FOnFadeDelegate)
	FOnFadeDelegate OnFadeResultDelegate;

protected:
	UPROPERTY() UImage* image_fade_;

	uint32 fading_ : 1;
	uint32 to_black_ : 1;
	float fade_alpha_;
	float fade_start_time_;
	float fade_duration_;
};
