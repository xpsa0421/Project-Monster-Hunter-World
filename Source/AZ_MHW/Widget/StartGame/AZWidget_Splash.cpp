// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/StartGame/AZWidget_Splash.h"

#include "Kismet/KismetSystemLibrary.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Login/AZLoginMgr.h"

void UAZWidget_Splash::Init()
{
	Super::Init();

	// ���� ȭ�� �����°�(�����) ����
	UKismetSystemLibrary::ControlScreensaver(false);

	splash_ani = GetWidgetAnimation(TEXT("Splash_Ani"));
}

void UAZWidget_Splash::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	if (splash_ani)
	{
		PlayAnimation(splash_ani);
	}

	game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::Splash);
}

void UAZWidget_Splash::OnAnimationFinished_Implementation(const UWidgetAnimation* animation)
{
	Super::OnAnimationFinished_Implementation(animation);

	if (animation == splash_ani)
	{
		game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::LoginPageStart);
	}
}
