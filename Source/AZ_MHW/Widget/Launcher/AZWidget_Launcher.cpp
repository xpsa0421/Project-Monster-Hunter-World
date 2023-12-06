// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Widget/Launcher/AZWidget_Launcher.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "TimerManager.h"

void UAZWidget_Launcher::Init()
{
	Super::Init();
}

void UAZWidget_Launcher::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);

	GetWorld()->GetTimerManager().SetTimer(OnBeginTimer, this, &UAZWidget_Launcher::OnBegin, 1.0f);
}

void UAZWidget_Launcher::NativeTick(const FGeometry& my_geometry, float in_delta_time)
{
	Super::NativeTick(my_geometry, in_delta_time);
}

void UAZWidget_Launcher::OnAnimationStarted_Implementation(const UWidgetAnimation* animation)
{
	Super::OnAnimationStarted_Implementation(animation);
}

void UAZWidget_Launcher::OnAnimationFinished_Implementation(const UWidgetAnimation* animation)
{
	Super::OnAnimationFinished_Implementation(animation);

	if (animation)
	{
		game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::LoginPageStart);
	}
}

void UAZWidget_Launcher::OnBegin()
{
}
