// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_Menu.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Menu : public UAZWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;

private:
	UFUNCTION() void OnClicked_Start();
	UFUNCTION() void OnClicked_Option();
	UFUNCTION() void OnClicked_Credits();
	UFUNCTION() void OnClicked_Close();

private:
	UPROPERTY(meta = (BindWidget)) class UButton* c_btn_start_;
	UPROPERTY(meta = (BindWidget)) class UButton* c_btn_option_;
	UPROPERTY(meta = (BindWidget)) class UButton* c_btn_credits_;
	UPROPERTY(meta = (BindWidget)) class UButton* c_btn_exit_;
};
