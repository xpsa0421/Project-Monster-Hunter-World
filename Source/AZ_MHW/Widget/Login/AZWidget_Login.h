// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_Login.generated.h"

class UButton;
class UEditableTextBox;

/**
 *
 */
UCLASS()
class AZ_MHW_API UAZWidget_Login : public UAZWidget
{
	GENERATED_BODY()

		enum class EPanelState : uint8
	{
		TouchScreen,
		IDPasswordLogin,
		Max
	};
public:

	enum class ELogInMode : uint8
	{
		TouchConnect,
		IDPassword
	};

	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	void SetLoginMode(ELogInMode login_mode);

private:
	UFUNCTION() void OnTouchAnyPress();
	UFUNCTION() void OnClicked_Login();
	UFUNCTION() void OnClicked_Close();
	UFUNCTION() void OnClicked_SignUp();

public:
	UPROPERTY() UWidget* panel_state_[(int32)EPanelState::Max];
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_login_;
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_any_press_;
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_close_;
	UPROPERTY(meta = (BindWidget)) UButton* c_btn_sign_up_;
	UPROPERTY(meta = (BindWidget)) UEditableTextBox* c_id_;
	UPROPERTY(meta = (BindWidget)) UEditableTextBox* c_pass_;
};
