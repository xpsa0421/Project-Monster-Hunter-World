// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidgetAction.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWidgetActionState : uint8
{
	None,

	ExecuteAction,
	ReleaseAction
};
UCLASS()
class AZ_MHW_API UAZWidgetAction : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UAZWidgetAction();
	virtual ~UAZWidgetAction();

public:
	virtual void Tick(float delta_time) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

public:
	virtual bool IsActionBegin() const;
	virtual bool IsActionFinished() const;
	virtual bool IsUseReleaseAction() const;

	void SetBegin(bool begin);
	virtual void SetFinished(bool finished);

public:
	float GetElapsedTime() const;

	template<typename T>
	void SetTargetWidgetWithCallFunc(class UWidget* target_widget, T* call_object, void (T::* OnExecuteActionEndCallFunc)(), void (T::* OnReleaseActionEndCallFunc)())
	{
		target_widget_ = target_widget;

		OnExecuteActionEndDelegate.Unbind();
		if (OnExecuteActionEndCallFunc != nullptr)
		{
			OnExecuteActionEndDelegate.BindUObject(call_object, OnExecuteActionEndCallFunc);
		}

		OnReleaseActionEndDelegate.Unbind();
		if (OnReleaseActionEndCallFunc != nullptr)
		{
			OnReleaseActionEndDelegate.BindUObject(call_object, OnReleaseActionEndCallFunc);
		}
	}

public:
	void SetTargetWidget(class UWidget* target_widget);
	class UWidget* GetTargetWidget();

public:
	void BeginAction(EWidgetActionState state);
	virtual void BeginExecuteAction();
	virtual void BeginReleaseAction();

protected:
	bool is_begin_;
	bool is_finished_;
	float elapsed_time_;
	EWidgetActionState action_state_;
	class UWidget* target_widget_;

protected:
	DECLARE_DELEGATE(FOnWidgetActionDelegate);
	FOnWidgetActionDelegate OnExecuteActionEndDelegate;
	FOnWidgetActionDelegate OnReleaseActionEndDelegate;

public:
	UPROPERTY(EditAnyWhere, Category = "WidgetAction") float excute_action_duration;
	UPROPERTY(EditAnyWhere, Category = "WidgetAction") float release_action_duration;
	UPROPERTY(EditAnyWHere, Category = "WidgetAction") bool is_release_action;
};
