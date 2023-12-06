#include "AZ_MHW/CommonSource/Table/ArmorData.h"

void UArmorData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("type");
	if (value_ptr != nullptr)
	{
		type = **value_ptr;
	}
	value_ptr = data.Find("defense");
	if (value_ptr != nullptr)
	{
		defense = FCString::Atoi(**value_ptr);
	}
}
