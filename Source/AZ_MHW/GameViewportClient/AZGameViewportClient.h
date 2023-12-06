// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "AZGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	virtual void Init(struct FWorldContext& world_context, UGameInstance* owning_game_instance, bool create_new_audio_device = true) override;

	virtual void PostRender(UCanvas* Canvas) override;

	virtual void Fade(const float duration, const bool in_to_black);
	virtual void FadeIn(const float fade_time, bool force = false);
	virtual void FadeOut(const float fade_time, bool force = false);
	virtual void FadeInout(const float fade_in_time, const float fade_out_time);

	void DrawScreenFade(UCanvas* canvas);
	bool IsFading() { return fading_; }

	DECLARE_EVENT(UAZGameViewportClient, FOnFadeDelegate);
	FOnFadeDelegate OnFadeResultDelegate;

private:
	uint32 fading_ : 1;
	uint32 to_black_ : 1; // applied even if alpha is 1
	float fade_alpha_;
	float fade_start_time_;
	float fade_duration_;

public:
	virtual void BeginDestroy() override;
};
