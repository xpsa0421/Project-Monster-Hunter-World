#include "AZ_MHW/CommonSource/Table/BottleData.h"

void UBottleData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("is_buff");
	if (value_ptr != nullptr)
	{
		is_buff = (*value_ptr).ToBool();
	}
}
