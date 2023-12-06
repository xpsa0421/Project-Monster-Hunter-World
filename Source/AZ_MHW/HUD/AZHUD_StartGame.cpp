// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUD_StartGame.h"

#include "AZ_MHW/Widget/StartGame/AZWidget_Splash.h"

void AAZHUD_StartGame::BeginPlay()
{
	Super::BeginPlay();

	UAZWidget_Splash* splash = OpenScene<UAZWidget_Splash>(EUIName::AZWidget_Splash);
	splash->SetIsDisableBackBtnExit(false);
}

void AAZHUD_StartGame::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD_StartGame::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);
}
