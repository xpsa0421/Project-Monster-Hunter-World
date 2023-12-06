// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_Loading.generated.h"

enum class EModelType : uint8
{
	Avatar,
	Monster,
	Npc,
	All
};

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Loading : public UAZWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void NativeTick(const FGeometry& my_geometry, float inf_delta_time) override;

	virtual bool OnWidgetOpenAction(bool isVisibility = false) override;

	void SetLoadingModel(const class UModelData* model_data, int32 mapIndex);

	UFUNCTION() void OnPressedTouchScreen();
	UFUNCTION() void OnReleasedTouchScreen();
	UFUNCTION() void OnMoveTouchScreen(const FGeometry& my_geometry, const FPointerEvent& mouse_event);

	void SetAvatarInfo(int32 avatar_index);
	void SetModelText(const class UModelData* model_data);

private:
	UPROPERTY() UWidgetAnimation* text_animation_;
	UPROPERTY() UWidgetAnimation* fade_out_animation_;

	UPROPERTY(EditAnywhere, Category = "AZ Loading")
	TArray<class UTexture2D*> loading_textures_;

	UPROPERTY() class UWidgetSwitcher* c_desc_widget_switcher_;

	UPROPERTY() class UImage* c_image_player_class_;
	UPROPERTY() class UAZTextBlock* c_text_avatar_class_;
	UPROPERTY() class UTextBlock* c_avatar_name_text_;
	UPROPERTY() class UTextBlock* c_avatar_job_name_text_;
	UPROPERTY() class UWidgetSwitcher* c_character_widget_switcher_;

	UPROPERTY() UImage* loading_bg_image_;

	UPROPERTY() class UAZButton* c_touch_screen_;
	uint8 touch_screen_pressed_ : 1;
	FVector last_touch_location_;

	UPROPERTY(EditAnywhere, Category = "AZ Loading")
	FVector spawn_location_;

	UPROPERTY(EditAnywhere, Category = "AZ Loading")
	float auto_rotate_value_;

	UPROPERTY() class UCanvasPanel* c_canvas_;
	UPROPERTY() class UImage* c_image_loading_actor_;

	UPROPERTY() class UAZTextBlock* c_text_name_;
	UPROPERTY() class UAZTextBlock* c_text_desc_;
	UPROPERTY() class UAZTextBlock* c_text_tip_;
	UPROPERTY() class UAZTextBlock* text_desc_;
	float cached_ratio_;
};
