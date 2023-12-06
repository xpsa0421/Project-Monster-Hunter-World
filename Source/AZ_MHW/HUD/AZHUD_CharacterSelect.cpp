// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUD_CharacterSelect.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterSelect.h"

void AAZHUD_CharacterSelect::BeginPlay()
{
	Super::BeginPlay();

	UAZWidget_CharacterSelect* character_select = OpenScene<UAZWidget_CharacterSelect>(EUIName::AZWidget_CharacterSelect);
	AZ_PRINT_LOG_IF_FALSE(character_select);
	character_select->SetIsDisableBackBtnExit(true);
}

void AAZHUD_CharacterSelect::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD_CharacterSelect::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);
}