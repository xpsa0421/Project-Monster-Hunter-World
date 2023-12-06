// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "Components/Image.h"
#include "AZWidget_Waiting.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Waiting : public UAZWidget
{
	GENERATED_BODY()

public:
	UAZWidget_Waiting();

	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void OnClose(bool immediately = false) override;

	static void OnForceWaiting() { is_force_waiting_ = true; }
	static void ClearForceWaiting() { is_force_waiting_ = false; }
	static bool GetIsOfrcedWaiting() { return is_force_waiting_; }

	UFUNCTION()
	void ShowWaitingImage();

private:
	UPROPERTY(meta = (BindWidget)) UImage* image_waiting_;
	UWidgetAnimation* waiting_ani_;
	static bool is_force_waiting_;
	FTimerHandle show_waiting_timer_handler_;
	FString sended_packet_name_;
};
