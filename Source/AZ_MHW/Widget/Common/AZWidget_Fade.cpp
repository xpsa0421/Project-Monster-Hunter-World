// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Widget/Common/AZWidget_Fade.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/HUD/AZHUD.h"



void UAZWidget_Fade::Init()
{
	Super::Init();

	AZ_PRINT_LOG_IF_FALSE(image_fade_);
}

void UAZWidget_Fade::NativeTick(const FGeometry& my_geometry, float in_delta_time)
{
	Super::NativeTick(my_geometry, in_delta_time);
	if (fading_ == true)
	{
		auto world = GetWorld();
		if (world != nullptr)
		{
			const float time = world->GetTimeSeconds();
			const float alpha = FMath::Clamp((time - fade_start_time_) / fade_duration_, 0.0f, 1.0f);

			FLinearColor fade_color = FLinearColor::Black;
			fade_color.A = to_black_ ? alpha : 1.0f - alpha;

			// FADE
			{
				image_fade_->SetColorAndOpacity(fade_color);
			}

			if (alpha == 1.0f)
			{
				fading_ = false;

				if (OnFadeResultDelegate.IsBound())
				{
					OnFadeResultDelegate.Broadcast();
					OnFadeResultDelegate.Clear();
				}
				if (fading_ == false)
				{
					OnExecuteClose();
				}
			}
		}
	}
}

void UAZWidget_Fade::Fade(const float duration, const bool in_to_black)
{
	fading_ = true;
	to_black_ = in_to_black;
	fade_duration_ = duration;
	fade_start_time_ = GetWorld()->GetTimeSeconds();
}

void UAZWidget_Fade::FadeIn(const float fade_time, bool force)
{
	float time = fade_time;
	if (force == true)
	{
		time = 0.0f;
		Fade(0.0f, true);
	}
	else
	{
		Fade(time, true);
	}
}

void UAZWidget_Fade::FadeOut(const float fade_time, bool force)
{
	float time = fade_time;
	if (force == true)
	{
		time = 0.0f;
		Fade(0.0f, false);
	}
	else
	{
		Fade(time, false);
	}
}

void UAZWidget_Fade::FadeInOut(const float fade_in_time, const float fade_out_time)
{
	FadeIn(fade_in_time);

	OnFadeResultDelegate.AddLambda([this, fade_out_time]() -> void {
		FadeOut(fade_out_time);
	});
}
