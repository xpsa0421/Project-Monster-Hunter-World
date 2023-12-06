// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/MsgBox/AZWidget_MsgBoxBase.h"
#include "Components/Button.h"
#include "AZWidget_MsgBoxBasic.generated.h"

UENUM(BlueprintType)
enum class EMsgEventButtonType : uint8
{
	Left,
	Right,
	Cancle,
	Close,
	Screen
};

UCLASS()
class AZ_MHW_API UAZWidget_MsgBoxBasic : public UAZWidget_MsgBoxBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void Destroy() override;

	void SetButtonType(EUIMsgBoxBtnType button_type);
	void SetButtonText(EMsgEventButtonType button_type, FString text);
	void ShowButton(EMsgEventButtonType button_type, bool isShow);

	DECLARE_DELEGATE(FCallMsgFuncOnTap);

	template<class T>
	void AddHandler(EMsgEventButtonType button_type, T* owner, const FName& function_name)
	{
		if (button_type == EMsgEventButtonType::Left) {
			handler_left_.Unbind();
			handler_left_.BindUFunction(owner, function_name);
		}
		else if (button_type == EMsgEventButtonType::Right) {
			handler_right_.Unbind();
			handler_right_.BindUFunction(owner, function_name);
		}
		else if (button_type == EMsgEventButtonType::Cancle) {
			handler_cancle_.Unbind();
			handler_cancle_.BindUFunction(owner, function_name);
		}
		else if (button_type == EMsgEventButtonType::Close) {
			handler_close_.Unbind();
			handler_close_.BindUFunction(owner, function_name);
		}
	}

public:
	UFUNCTION() void OnTapClose_FromOutside();
	void CloseMsgBox();
	
protected:
	virtual FReply NativeOnKeyDown(const FGeometry& in_geometry, const FKeyEvent& in_key_event) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& in_geometry, const FPointerEvent& in_mouse_event) override;

	UFUNCTION(BlueprintCallable, Category = "UAZWidget_MsgBoxBasic") virtual void OnTapLeft();
	UFUNCTION(BlueprintCallable, Category = "UAZWidget_MsgBoxBasic") virtual void OnTapRight();
	UFUNCTION(BlueprintCallable, Category = "UAZWidget_MsgBoxBasic") virtual void OnTapCancle();
	UFUNCTION(BlueprintCallable, Category = "UAZWidget_MsgBoxBasic") virtual void OnTapClose();

	UPROPERTY() TArray<UButton*> button_;
	UPROPERTY() TArray<UTextBlock*> button_text_;

	UPROPERTY(meta = (BindWidget)) class UHorizontalBox* c_full_screen_box_;

protected:
	EUIMsgBoxBtnType button_type_;
	FCallMsgFuncOnTap handler_left_;
	FCallMsgFuncOnTap handler_right_;
	FCallMsgFuncOnTap handler_cancle_;
	FCallMsgFuncOnTap handler_close_;

	UPROPERTY(meta = (BindWidget)) UButton* c_btn_left_;
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_right_;
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_cancle_;
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_close_;
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_fill_screen_;

	UPROPERTY(meta = (BindWidget)) UTextBlock* c_text_left_btn_;
	UPROPERTY(meta = (BindWidget)) UTextBlock* c_text_right_btn_;

	UWidgetAnimation* c_ani_;
};
