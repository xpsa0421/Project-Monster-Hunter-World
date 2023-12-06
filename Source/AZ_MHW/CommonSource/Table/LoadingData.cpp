#include "AZ_MHW/CommonSource/Table/LoadingData.h"

void ULoadingData::Load(TMap<FString, FString>& data)
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
	value_ptr = data.Find("model_type");
	if (value_ptr != nullptr)
	{
		model_type = **value_ptr;
	}
	value_ptr = data.Find("monster_group_id");
	if (value_ptr != nullptr)
	{
		monster_group_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("npc_group_id");
	if (value_ptr != nullptr)
	{
		npc_group_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("tip_group_id");
	if (value_ptr != nullptr)
	{
		tip_group_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("location_x");
	if (value_ptr != nullptr)
	{
		location_x = FCString::Atof(**value_ptr);
	}
	value_ptr = data.Find("location_y");
	if (value_ptr != nullptr)
	{
		location_y = FCString::Atof(**value_ptr);
	}
	value_ptr = data.Find("location_z");
	if (value_ptr != nullptr)
	{
		location_z = FCString::Atof(**value_ptr);
	}
}
