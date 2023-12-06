// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/System/WidgetAction/AZWidgetAction.h"

UAZWidgetAction::UAZWidgetAction()
{
	is_finished_ = true;
	is_begin_ = false;
	excute_action_duration = 0.0f;
	release_action_duration = 0.0f;
}

UAZWidgetAction::~UAZWidgetAction()
{
	if (OnExecuteActionEndDelegate.IsBound() == true)
	{
		OnExecuteActionEndDelegate.Unbind();
	}
	if (OnReleaseActionEndDelegate.IsBound() == true)
	{
		OnReleaseActionEndDelegate.Unbind();
	}
}

void UAZWidgetAction::Tick(float delta_time)
{

}

bool UAZWidgetAction::IsTickable() const
{
	return !HasAnyFlags(EObjectFlags::RF_ClassDefaultObject);
}

TStatId UAZWidgetAction::GetStatId() const
{
	return TStatId();
}

bool UAZWidgetAction::IsActionBegin() const
{
	return is_begin_;
}

bool UAZWidgetAction::IsActionFinished() const
{
	return is_finished_;
}

bool UAZWidgetAction::IsUseReleaseAction() const
{
	return is_release_action;
}

void UAZWidgetAction::SetBegin(bool begin)
{
	is_begin_ = begin;
}

void UAZWidgetAction::SetFinished(bool finished)
{
	is_finished_ = finished;

	SetBegin(!is_finished_);

	if (is_finished_ == true)
	{
		if (action_state_ == EWidgetActionState::ExecuteAction && OnExecuteActionEndDelegate.IsBound())
		{
			OnExecuteActionEndDelegate.ExecuteIfBound();
		}
		else if (action_state_ == EWidgetActionState::ReleaseAction && OnReleaseActionEndDelegate.IsBound())
		{
			OnReleaseActionEndDelegate.ExecuteIfBound();
		}
	}
}

float UAZWidgetAction::GetElapsedTime() const
{
	return elapsed_time_;
}

void UAZWidgetAction::SetTargetWidget(UWidget* target_widget)
{
	target_widget_ = target_widget;
}

UWidget* UAZWidgetAction::GetTargetWidget()
{
	return target_widget_;
}

void UAZWidgetAction::BeginAction(EWidgetActionState state)
{
	SetBegin(true);
	SetFinished(false);

	elapsed_time_ = 0.0f;

	action_state_ = state;
}

void UAZWidgetAction::BeginExecuteAction()
{
	BeginAction(EWidgetActionState::ExecuteAction);
}

void UAZWidgetAction::BeginReleaseAction()
{
	BeginAction(EWidgetActionState::ReleaseAction);
}
