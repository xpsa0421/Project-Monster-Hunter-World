// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AZ_MHW/HUD/AZHUDDefine.h"
#include "AZ_MHW/Widget/AZWidgetData.h"
//#include "AZ_MHW/Widget/System/WidgetAction/AZWidgetAction.h"
#include <Animation/WidgetAnimation.h>
#include <Components/SlateWrapperTypes.h>
#include "AZWidget.generated.h"

/**
 * 
 */

enum class EUIName : uint16;
UCLASS(Abstract)
class AZ_MHW_API UAZWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnWidgetClosed, bool);
public:
	//UAZWidget(const class FObjectInitializer& object_Initializer);
	//change construct
	UAZWidget(FVTableHelper& helper);

	//  AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다.
	virtual void NativeConstruct() override;

	virtual void SetVisiblity(ESlateVisibility in_visiblilty);
	virtual void SetVisibilitySkipAction(ESlateVisibility in_visibility);

	class AAZGameMode* GetGameMode();
	class AAZHUD* GetHUD();
	// FIXME (병합 후 구현하기) // 임시구현
	APlayerController* GetPlayerController();
	//class ALHPlayerController* GetPlayerController();
	//class ALHPlayerController_InGame* GetPlayerController_InGame();

	// FIXME (병합 후 구현하기)
	//class AAZPlayer_Playable* GetPlayer();

	void SetWidgetNameEnum(EUIName name) { ui_name_ = name; }
	void StopAnimToOrigin(UWidgetAnimation* anim);
	EUIName GetWidgetNameEnum() { return ui_name_; }

	//AZHUD에서 OpenUI로 위젯이 생성될 경우 자동 호출.
	UFUNCTION(BlueprintCallable, Category = "AZ")
	virtual void Init();

	// UI가 Open될때 마다
	virtual void OnOpen(bool immediately = false);
	virtual void OnExecuteOpen();

	// UI가 Open될때 마다
	virtual void OnClose(bool immediately = false);
	virtual void OnExecuteClose();

	// Open애니메이션 끝났을때 불림
	UFUNCTION()
	virtual void OnOpenAnimationFinished();
	virtual void ActivateContentsTutorial();

	// UI가 Close될 때마다. CloseScene 할 경우, 파라미터에 따라 OnClose가 아닌 ForceHide가 호출 됨
	UFUNCTION()
	virtual void OnCloseAnimationFinished();
	virtual void OnVisibilityClose();
	virtual void Destroy();
	void InitOption();

	void PlayAnimationLoop(UWidgetAnimation* anim, float play_back_speed = 1.0f);
	void OnOpenLoopAnim();

	UWidgetAnimation* GetWidgetAnimation(const FString& animation_name);

	virtual bool OnWidgetOpenAction(bool is_visibility = false);
	virtual bool OnWidgetCloseAction();

	FOnWidgetClosed widget_close_event;
	virtual void RestoreFromForceHidden();

	EUIState GetOpenUIState() { return open_ui_state_; }

	virtual void ForceHide();
	virtual void OnCoveredUI() {};

	bool GetIsForceHidden() { return is_force_hidden_; }

	void SetIsDisableBackBtnExit(bool is_disable_back_btn_exit) { is_disable_back_btn_exit_ = is_disable_back_btn_exit; }
	bool GetIsDisableBackBtnExit() { return is_disable_back_btn_exit_; }

	// AZHUD 에서 OpenScene으로 위젯을 열때, Scene 스택에서 복원할 경우 자동 호출, 나머지는 수동 호출
	virtual void Update() {}
	virtual void Reset() {}

	virtual void EditorOnly_Refresh() {}
	virtual bool DifferenceBackButton() { return false; }

	// touch screen and widget check (empty space check)
	virtual void OnTouchEmptySpace() {};

	virtual int32 GetSearchTouchMaskContentIndex(UWidget* widget, ETouchMaskSearchType search_type, int32 index);

	template<class T>
	T* GetOwnWidget(const FString& widget_name)
	{
		T* widget = Cast<T>(GetWidgetFromName(FName(*widget_name)));
		if (!widget)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetOwnWidget Fail"));
		}
		return widget;
	}

private:
	bool is_inited_ = false;
	EUIName ui_name_;
	EUIState open_ui_state_;
	ESlateVisibility visibility_beforce_force_hidden_;
	bool is_force_hidden_ = false;
	bool is_disable_back_btn_exit_ = false;
	int32 ui_layer_;
	float open_alpha_value_;
	FTimerHandle timer_handle_;
public:
	uint8 is_scene_ui : 1;

private:
	FOnWidgetClosed widget_close_event_;

protected:
	class UWidgetAnimation* open_ani;
	class UWidgetAnimation* close_ani;

protected:
	UPROPERTY(EditAnywhere, Category = AZWidget)
	bool is_widget_action_;

	UPROPERTY(EditAnywhere, Category = AZWidget)
	FString target_widget_name_;

	UPROPERTY(EditAnywhere, Category = AZWidget)
	TSubclassOf<class UAZWidgetAction> widget_action_class_;

	UPROPERTY() class UAZWidgetAction* widget_open_action_;

	UFUNCTION() void OpenAlphaTimeCallBack();
	virtual void OnAlphaTimeFinished();

	UPROPERTY() class UAZGameInstance* game_instance_;
};
