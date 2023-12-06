// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Widget/Login/AZWidget_Loading.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Environment/AZActor_LoadingScene.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/CanvasPanelSlot.h"
#include "ShaderPipelineCache.h"


void UAZWidget_Loading::Init()
{
	Super::Init();

	text_animation_ = GetWidgetAnimation(TEXT("Text_Ani"));
	fade_out_animation_ = GetWidgetAnimation(TEXT("Fade_Out"));
	loading_bg_image_ = GetOwnWidget<UImage>(TEXT("Image_0"));

	c_touch_screen_->OnPressed.Clear();
	c_touch_screen_->OnPressed.AddDynamic(this, &UAZWidget_Loading::OnPressedTouchScreen);
	c_touch_screen_->OnReleased.Clear();
	c_touch_screen_->OnReleased.AddDynamic(this, &UAZWidget_Loading::OnReleasedTouchScreen);
	//c_touch_screen_->OnMouseMoveEvent.Unbind();
	//c_touch_screen_->OnMouseMoveEvent.BindUObject(this, &UAZWidget_Loading::OnMoveTouchScreen);
}

void UAZWidget_Loading::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	cached_ratio_ = 0.f;

	FShaderPipelineCache::SetBatchMode(FShaderPipelineCache::BatchMode::Fast);
}

void UAZWidget_Loading::NativeTick(const FGeometry& my_geometry, float in_delta_time)
{
	Super::NativeTick(my_geometry, in_delta_time);

	if (c_touch_screen_->IsPressed() == false)
		OnMoveTouchScreen(my_geometry, FPointerEvent());

	auto canvas_rect = c_canvas_->GetCachedGeometry().GetLayoutBoundingRect().GetSize();
	if (FMath::IsNearlyZero(canvas_rect.X) || FMath::IsNearlyZero(canvas_rect.Y))
		return;

	auto scaleVector = c_canvas_->GetCachedGeometry().GetAccumulatedRenderTransform().GetMatrix().GetScale().GetVector();
	if (FMath::IsNearlyZero(scaleVector.X) || FMath::IsNearlyZero(scaleVector.Y))
		return;

	auto canvasRenderRect = canvas_rect / scaleVector;
	float canvasX = canvasRenderRect.X;
	float canvasY = canvasRenderRect.Y;
	float canvasRatio = canvasX / canvasY;

	if (FMath::IsNearlyEqual(canvasRatio, cached_ratio_))
		return;

	cached_ratio_ = canvasRatio;

	if (auto* mi = Cast<UMaterialInstance>(c_image_loading_actor_->Brush.GetResourceObject()))
	{
		FMaterialParameterInfo ParameterInfo(TEXT("LoadingTexture"));
		UTexture* texture = nullptr;
		if (mi->GetTextureParameterValue(ParameterInfo, texture))
		{
			if (auto* rttex = Cast<UTextureRenderTarget2D>(texture))
			{
				float textureX = rttex->SizeX;
				float textureY = rttex->SizeY;
				float textureRatio = textureX / textureY;

				float targetX = 1.f, targetY = 1.f;

				if (textureRatio > canvasRatio)
				{
					targetX = canvasY * textureRatio;
					targetY = canvasY;
				}
				else
				{
					targetX = canvasX;
					targetY = canvasX / textureRatio;
				}

				if (auto* slot = Cast<UCanvasPanelSlot>(c_image_loading_actor_->Slot))
				{
					slot->SetSize(FVector2D(targetX, targetY));
				}
			}
		}
	}
}

bool UAZWidget_Loading::OnWidgetOpenAction(bool isVisibility)
{
	return true;
}

void UAZWidget_Loading::SetLoadingModel(const UModelData* model_data, int32 mapIndex)
{
	// FIXME animation tooltip, avartar apper
	/*PlayAnimation(fade_out_animation);
	PlayAnimationLoop(text_animation);

	loading_bg_image->SetVisibility(ESlateVisibility::Collapsed);
	c_image_loading_actor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	c_desc_widget_switcher->SetActiveWidgetIndex(Avatar);
	SetModelText(modle_data);*/

	// FIXME loading animation tooltip, avartar apper
	//int32 level = 1;
	//if (GetPlayer() && GetPlayer()->CharacterParam)
	//	level = GetPlayer()->CharacterParam->GetLevel();

	//TArray<int32> tipStringCodeList;
	//for (auto* tipRec : GetTable(ULoadingTable_Client)->m_TipInfoSheet->arrayTable)
	//{
	//	if (auto* loadingInfo = GetTable(ULoadingTable_Client)->GetLoadingInfoFromMapIndex(mapIdx))
	//	{
	//		if (loadingInfo->TipGroupID == tipRec->TipGroup && level >= tipRec->LevelLimit)
	//			tipStringCodeList.Add(tipRec->StringCode);
	//	}
	//}



	//if (tipStringCodeList.Num() > 0 && IsValid(C_Text_Tip))
	//{
	//	int32 selected = FMath::Rand() % tipStringCodeList.Num();
	//	C_Text_Tip->SetTextTableID(tipStringCodeList[selected]);
	//}
}

void UAZWidget_Loading::OnPressedTouchScreen()
{
	touch_screen_pressed_ = true;
}

void UAZWidget_Loading::OnReleasedTouchScreen()
{
	touch_screen_pressed_ = false;
}

void UAZWidget_Loading::OnMoveTouchScreen(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{
	auto* loading_scene_actor_instance = game_instance_->map_mgr->GetLoadingSceneActor();

	if (IsValid(loading_scene_actor_instance) == false)
		AZ_LOG("[UAZWidget_Loading] Miss Loading Scene Actor!!!");

	auto dragDelta = FVector2D::ZeroVector;
	if (c_touch_screen_->IsPressed())
	{
		dragDelta = mouse_event.GetCursorDelta();
		auto widgetRectSize = my_geometry.GetLayoutBoundingRect().GetSize();
		dragDelta.X /= widgetRectSize.X;
		dragDelta.Y /= widgetRectSize.Y;
	}
	else
	{
		auto widgetRectSize = my_geometry.GetLayoutBoundingRect().GetSize();
		dragDelta.X = auto_rotate_value_ / widgetRectSize.X;
	}

	//TODO 추후구현
	/*if (IsValid(loading_scene_actor_instance))
		loading_scene_actor_instance->AddInputVector(dragDelta);*/
}
