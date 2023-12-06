// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUD_Login.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Widget/Login/AZWidget_Login.h"

void AAZHUD_Login::BeginPlay()
{
	Super::BeginPlay();

	UAZWidget_Login* login_page = OpenScene<UAZWidget_Login>(EUIName::AZWidget_Login);
	AZ_PRINT_LOG_IF_FALSE(login_page);
	login_page->SetIsDisableBackBtnExit(false);
}

void AAZHUD_Login::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD_Login::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);
}
