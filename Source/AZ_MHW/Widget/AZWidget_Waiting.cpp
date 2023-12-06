// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/AZWidget_Waiting.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/CommonSource/AZLog.h"

bool UAZWidget_Waiting::is_force_waiting_ = false;

UAZWidget_Waiting::UAZWidget_Waiting()
{
	SetIsDisableBackBtnExit(true);
}

void UAZWidget_Waiting::Init()
{
	Super::Init();

	waiting_ani_ = GetWidgetAnimation(TEXT("Waiting_Ani"));
}

void UAZWidget_Waiting::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);

	image_waiting_->SetVisibility(ESlateVisibility::Collapsed);

	GetWorld()->GetTimerManager().SetTimer(show_waiting_timer_handler_, this, &UAZWidget_Waiting::ShowWaitingImage, 0.0f, false);
}

void UAZWidget_Waiting::OnClose(bool immediately)
{
	Super::OnClose(immediately);
	StopAnimToOrigin(waiting_ani_);

	GetWorld()->GetTimerManager().ClearTimer(show_waiting_timer_handler_);
}

void UAZWidget_Waiting::ShowWaitingImage()
{
	image_waiting_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (waiting_ani_ != nullptr)
	{
		PlayAnimation(waiting_ani_, 0.0f, 0);
	}
}
