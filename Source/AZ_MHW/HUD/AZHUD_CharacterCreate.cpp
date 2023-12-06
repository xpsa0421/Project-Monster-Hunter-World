// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUD_CharacterCreate.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterCreate.h"

void AAZHUD_CharacterCreate::BeginPlay()
{
	Super::BeginPlay();

	UAZWidget_CharacterCreate* character_create = OpenScene<UAZWidget_CharacterCreate>(EUIName::AZWidget_CharacterCreate);
	AZ_PRINT_LOG_IF_FALSE(character_create);
	character_create->SetIsDisableBackBtnExit(true);
}

void AAZHUD_CharacterCreate::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD_CharacterCreate::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);
}
