#include "AZ_MHW/CommonSource/Table/SpawnData.h"

void USpawnData::Load(TMap<FString, FString>& data)
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
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			map_index.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("drop_group_id");
	if (value_ptr != nullptr)
	{
		drop_group_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("spawn_count");
	if (value_ptr != nullptr)
	{
		spawn_count = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("patrol_range");
	if (value_ptr != nullptr)
	{
		patrol_range = FCString::Atof(**value_ptr);
	}
	value_ptr = data.Find("obj_index");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			obj_index.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("is_active");
	if (value_ptr != nullptr)
	{
		is_active = (*value_ptr).ToBool();
	}
}
