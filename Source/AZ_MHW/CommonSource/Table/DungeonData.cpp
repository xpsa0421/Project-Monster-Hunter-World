#include "AZ_MHW/CommonSource/Table/DungeonData.h"

void UDungeonData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("map_index");
	if (value_ptr != nullptr)
	{
		map_index = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("target_index");
	if (value_ptr != nullptr)
	{
		target_index = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("area_index");
	if (value_ptr != nullptr)
	{
		area_index = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("goal_string");
	if (value_ptr != nullptr)
	{
		goal_string = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("is_boss");
	if (value_ptr != nullptr)
	{
		is_boss = (*value_ptr).ToBool();
	}
}
