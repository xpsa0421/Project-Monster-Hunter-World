#include "AZ_MHW/CommonSource/Table/MapData.h"

void UMapData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("map_bg");
	if (value_ptr != nullptr)
	{
		map_bg = **value_ptr;
	}
	value_ptr = data.Find("minimap_image");
	if (value_ptr != nullptr)
	{
		minimap_image = **value_ptr;
	}
	value_ptr = data.Find("map_3d_model");
	if (value_ptr != nullptr)
	{
		map_3d_model = **value_ptr;
	}
	value_ptr = data.Find("map_type");
	if (value_ptr != nullptr)
	{
		map_type = **value_ptr;
	}
	value_ptr = data.Find("max_user");
	if (value_ptr != nullptr)
	{
		max_user = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("target_index");
	if (value_ptr != nullptr)
	{
		target_index = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("map_name");
	if (value_ptr != nullptr)
	{
		map_name = **value_ptr;
	}
}
