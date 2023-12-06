// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/AZWidget.h"

#include "AZ_MHW/Widget/System/WidgetAction/AZWidgetAction.h"
#include "MovieScene.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/HUD/AZHUD.h"

//UAZWidget::UAZWidget(const FObjectInitializer& object_initializer)
//    :Super(object_initializer)
//{
//    bHasScriptImplementedPaint = false;
//    bHasScriptImplementedPaint = false;
//    is_inited_ = false;
//}

UAZWidget::UAZWidget(FVTableHelper& helper) : Super(helper)
{
    bHasScriptImplementedTick = false;
    bHasScriptImplementedPaint = false;
    is_inited_ = false;
}

void UAZWidget::NativeConstruct()
{
    Super::NativeConstruct();

    OnOpenLoopAnim();
}

void UAZWidget::SetVisiblity(ESlateVisibility in_visibillty)
{
    if ((GetVisibility() == ESlateVisibility::Collapsed || GetVisibility() == ESlateVisibility::Hidden) &&
        (in_visibillty != ESlateVisibility::Collapsed && in_visibillty != ESlateVisibility::Hidden))
    {
        OnWidgetOpenAction(true);
        UWidget::SetVisibility(in_visibillty);
    }
    else if ((GetVisibility() != ESlateVisibility::Collapsed && GetVisibility() != ESlateVisibility::Hidden) &&
        (in_visibillty == ESlateVisibility::Collapsed || in_visibillty == ESlateVisibility::Hidden))
    {
        if (OnWidgetCloseAction() == false)
        {
            UWidget::SetVisibility(in_visibillty);
        }
    }
}

void UAZWidget::SetVisibilitySkipAction(ESlateVisibility in_visibility)
{
    UWidget::SetVisibility(in_visibility);
}

AAZGameMode* UAZWidget::GetGameMode()
{
    return GetWorld() ? Cast<AAZGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
}

AAZHUD* UAZWidget::GetHUD()
{
    APlayerController* player_controller = GetOwningPlayer();
    if (player_controller == nullptr)
    {
        return nullptr;
    }

    AAZHUD* hud = Cast<AAZHUD>(player_controller->GetHUD());
    if (hud == nullptr)
    {
        return nullptr;
    }

    return hud;
}

APlayerController* UAZWidget::GetPlayerController()
{
    APlayerController* player_controller = GetOwningPlayer();
    // ���� ����
    if (player_controller == nullptr)
    {
        return nullptr;
    }

    return player_controller;
}

void UAZWidget::StopAnimToOrigin(UWidgetAnimation* anim)
{
    if (anim == nullptr)
    {
        return;
    }
    PlayAnimation(anim, anim->GetEndTime(), 1, EUMGSequencePlayMode::Reverse);
}

void UAZWidget::Init()
{
    game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
    open_ani = GetWidgetAnimation(TEXT("Open"));

    if (open_ani != nullptr)
    {
        FWidgetAnimationDynamicEvent open_animation_finish_event;
        open_animation_finish_event.BindDynamic(this, &UAZWidget::OnOpenAnimationFinished);
        BindToAnimationFinished(open_ani, open_animation_finish_event);
    }

    close_ani = GetWidgetAnimation(TEXT("Close"));

    if (close_ani != nullptr)
    {
        FWidgetAnimationDynamicEvent close_animation_finish_event;
        close_animation_finish_event.BindDynamic(this, &UAZWidget::OnCloseAnimationFinished);
        BindToAnimationFinished(close_ani, close_animation_finish_event);
    }

    InitOption();
    //FIXME ��ư����Ʈ�� ���� �ʿ����� Ȯ���ϱ�
    /*for (int i = 0; i < button_list.Num(); ++i)
    {
        button_list->InitOption();
    }*/
}

void UAZWidget::OnOpen(bool immediately)
{
    open_ui_state_ = EUIState::Opened;

    if (OnWidgetOpenAction() == false)
    {
        if (open_ani != nullptr)
        {
            PlayAnimation(open_ani);
        }
        else
        {
            if (immediately == false)
            {
                open_alpha_value_ = -0.2f;
                OpenAlphaTimeCallBack();
            }
        }
    }

    // FIXME (���ս� ��ǲ �Ŵ��� üũ�ϱ�)
    //AZGameInstance->input_mgr->CheckSceneStack();
}

void UAZWidget::OnExecuteOpen()
{
    ActivateContentsTutorial();
}

void UAZWidget::OnClose(bool immediately)
{
    open_ui_state_ = EUIState::Closed;
    if (OnWidgetCloseAction() == false)
    {
        if (close_ani != nullptr)
        {
            if (open_ani && IsAnimationPlaying(open_ani))
            {
                StopAnimation(open_ani);
            }

            PlayAnimation(close_ani);
        }
        else
        {
            OnExecuteClose();
        }
    }
    widget_close_event_.ExecuteIfBound(immediately);

    // FIXME (���ս� ��ǲ �Ŵ��� üũ�ϱ�)
    //AZGameInstance->input_mgr->CheckSceneStack();
}

void UAZWidget::OnExecuteClose()
{
    GetWorld()->GetTimerManager().ClearTimer(timer_handle_);
    Destroy();
}

void UAZWidget::OnOpenAnimationFinished()
{
    ActivateContentsTutorial();
}

void UAZWidget::ActivateContentsTutorial()
{
    //Ʃ�丮�� ����ũ ����
}

void UAZWidget::OnCloseAnimationFinished()
{
    OnExecuteClose();
}

void UAZWidget::OnVisibilityClose()
{
    UWidget::SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget::Destroy()
{
    // �������� �Լ����� RemoveFromParent�� ���� ���� �� Ȯ�� �ʿ�
    //RemoveFromViewport();
    RemoveFromParent();
    RestoreFromForceHidden();
}

void UAZWidget::InitOption()
{
}

void UAZWidget::PlayAnimationLoop(UWidgetAnimation* anim, float play_back_speed)
{
    if (anim == nullptr)
    {
        return;
    }
    PlayAnimation(anim, 0.0f, 0, EUMGSequencePlayMode::Forward, play_back_speed);
}

void UAZWidget::OnOpenLoopAnim()
{
    //�ǽð����� �������Ʈ�� ���� �� ����
    if (UWidgetBlueprintGeneratedClass* blueprint = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
    {
        for (auto anim : blueprint->Animations)
        {
            if (anim->MovieScene->GetName().Find(TEXT("BeginLoop")) != -1)
            {
                PlayAnimationLoop(anim);
            }
        }
    }
}

UWidgetAnimation* UAZWidget::GetWidgetAnimation(const FString& animation_name)
{
    if (UWidgetBlueprintGeneratedClass* blueprint = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
    {
        for (UWidgetAnimation* anim : blueprint->Animations)
        {
            if (anim->MovieScene->GetName().Equals(animation_name, ESearchCase::IgnoreCase))
            {
                return anim;
            }
        }
    }
    return nullptr;
}

bool UAZWidget::OnWidgetOpenAction(bool is_visibility)
{
    bool is_widget_action = false;
    if (is_widget_action_)
    {
        UWidget* find_target_widget = GetWidgetFromName(FName(*target_widget_name_));
        if (find_target_widget != nullptr && widget_action_class_ != nullptr)
        {
            if (widget_open_action_ == nullptr)
            {
                FStaticConstructObjectParameters params(widget_action_class_);
                params.Class = widget_action_class_;
                params.Outer = this;
                widget_open_action_ = Cast<UAZWidgetAction>(StaticConstructObject_Internal(params));
                if (is_visibility)
                {
                    widget_open_action_->SetTargetWidgetWithCallFunc(find_target_widget, this, &UAZWidget::OnExecuteOpen, &UAZWidget::OnVisibilityClose);
                }
                else
                {
                    widget_open_action_->SetTargetWidgetWithCallFunc(find_target_widget, this, &UAZWidget::OnExecuteOpen, &UAZWidget::OnExecuteClose);
                }
            }

            if (widget_open_action_ != nullptr)
            {
                is_widget_action = true;
                widget_open_action_->BeginExecuteAction();
            }
        }
    }
    return is_widget_action;
}

bool UAZWidget::OnWidgetCloseAction()
{
    if (is_widget_action_ && widget_open_action_ && widget_open_action_->IsUseReleaseAction())
    {
        if (widget_open_action_)
        {
            widget_open_action_->BeginReleaseAction();
            return true;
        }
    }
    return false;
}

void UAZWidget::OpenAlphaTimeCallBack()
{
    open_alpha_value_ += 0.1f;
    if (open_alpha_value_ >= 1.0f)
    {
        open_alpha_value_ = 1.0f;
        OnAlphaTimeFinished();
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(timer_handle_, this, &UAZWidget::OpenAlphaTimeCallBack, 0.025f);
    }

    SetRenderOpacity(open_alpha_value_);
}

void UAZWidget::OnAlphaTimeFinished()
{
    ActivateContentsTutorial();
}

void UAZWidget::RestoreFromForceHidden()
{
    if (is_force_hidden_ == true)
    {
        is_force_hidden_ = false;
        auto root_widget = GetRootWidget();
        root_widget->SetVisibility(visibility_beforce_force_hidden_);
    }
}

void UAZWidget::ForceHide()
{
    if (is_force_hidden_ == false)
    {
        is_force_hidden_ = true;
        visibility_beforce_force_hidden_ = GetRootWidget()->GetVisibility();
        GetRootWidget()->SetVisibility(ESlateVisibility::Collapsed);
    }
}

int32 UAZWidget::GetSearchTouchMaskContentIndex(UWidget* widget, ETouchMaskSearchType search_type, int32 index)
{
    if (search_type == ETouchMaskSearchType::SlotIndex)
    {
        return index - 1;
    }
    return INDEX_NONE;
}
