// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/GameViewportClient/AZGameViewportClient.h"

#include "Engine/Canvas.h"

void UAZGameViewportClient::Init(FWorldContext& world_context, UGameInstance* owning_game_instance, bool create_new_audio_device)
{
	Super::Init(world_context, owning_game_instance, create_new_audio_device);
}

void UAZGameViewportClient::PostRender(UCanvas* canvas)
{
	Super::PostRender(canvas);
	if (fading_ == true)
	{
		if (fading_)
		{
			DrawScreenFade(canvas);
		}
	}
}

void UAZGameViewportClient::Fade(const float duration, const bool in_to_black)
{
	if (World != nullptr)
	{
		fading_ = true;
		to_black_ = in_to_black;
		fade_duration_ = duration;
		fade_start_time_ = World->GetTimeSeconds();
	}
}

void UAZGameViewportClient::FadeIn(const float fade_time, bool force)
{
	float time = fade_time;
	if (force)
	{
		time = 0.0f;
		Fade(0.0f, true);
	}
	else
	{
		Fade(time, true);
	}
}

void UAZGameViewportClient::FadeOut(const float fade_time, bool force)
{
	float time = fade_time;
	if (force)
	{
		time = 0.0f;
		Fade(0.0f, false);
	}
	else
	{
		Fade(time, false);
	}
}

void UAZGameViewportClient::FadeInout(const float fade_in_time, const float fade_out_time)
{
	FadeIn(fade_in_time);

	OnFadeResultDelegate.AddLambda([this, fade_out_time]()->void {
		FadeOut(fade_out_time);
		});
}

void UAZGameViewportClient::DrawScreenFade(UCanvas* canvas)
{
	if (fading_ == true)
	{
		if (World)
		{
			const float time = World->GetTimeSeconds();
			const float alpha = FMath::Clamp((time - fade_start_time_) / fade_duration_, 0.0f, 1.0f);
			{
				FColor old_color = canvas->DrawColor;
				FLinearColor fade_color = FLinearColor::Black;
				fade_color.A = to_black_ ? alpha : 1 - alpha;

				canvas->DrawColor = fade_color.ToFColor(true);

				FIntPoint viewport_xy(Viewport->GetSizeXY());

				float clip_x = viewport_xy.X;
				float clip_y = viewport_xy.Y;
				float size_x = canvas->DefaultTexture->GetSizeX();
				float size_y = canvas->DefaultTexture->GetSizeY();

				canvas->DrawTile(
					canvas->DefaultTexture,
					0,
					0,
					clip_x,
					clip_y,
					0,
					0,
					size_x,
					size_y
				);

				if (alpha == 1.0f)
				{
					fading_ = false;
					if (OnFadeResultDelegate.IsBound())
					{
						OnFadeResultDelegate.Broadcast();
						OnFadeResultDelegate.Clear();
					}
				}
			}
		}
	}
}

void UAZGameViewportClient::BeginDestroy()
{
	Super::BeginDestroy();
}
