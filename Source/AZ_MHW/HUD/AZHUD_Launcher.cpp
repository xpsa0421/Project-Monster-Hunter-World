// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUD_Launcher.h"
#include "AZ_MHW/Widget/Launcher/AZWidget_Launcher.h"
#include "AZ_MHW/CommonSource/AZLog.h"

//FIXME DEVMODE 삭제할 것
#include "Widget/Login/AZWidget_Login.h"

void AAZHUD_Launcher::BeginPlay()
{
	Super::BeginPlay();

	//FIXME DEVMODE 삭제할 것
	UAZWidget_Login* login_page = OpenScene<UAZWidget_Login>(EUIName::AZWidget_Login);
	//FIXME DEVMODE 각주해제할 것
	//UAZWidget_Launcher* launcher = OpenScene<UAZWidget_Launcher>(EUIName::AZWidget_Scene_Launcher);

	//얘는 전에도 각주상태엿음
	//if(launcher) launcher->SetIsDisableBackBtnExit(false);
}

void AAZHUD_Launcher::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD_Launcher::EndPlay(const EEndPlayReason::Type end_paly_reason)
{
	Super::EndPlay(end_paly_reason);
}
