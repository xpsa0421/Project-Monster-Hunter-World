// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZSaveData.h"

#include "AZ_MHW/Manager/SaveData/GameOptionSaveData.h"
#include <Kismet/GameplayStatics.h>

UAZSaveData::UAZSaveData()
{
}

void UAZSaveData::LoadAll()
{
	option_save_data = NewObject<UOptionSaveData>();
	UOptionSaveData* op_save_data = Cast<UOptionSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("OptionSaveData"), 0));
	if (op_save_data == nullptr)
	{
		op_save_data = Cast<UOptionSaveData>(UGameplayStatics::CreateSaveGameObject(UOptionSaveData::StaticClass()));
	}
	option_save_data->game_option_save_data = op_save_data->game_option_save_data;
}

void UAZSaveData::SaveOptionData()
{
	UGameplayStatics::SaveGameToSlot(option_save_data, TEXT("OptionSaveData"), 0);
}
