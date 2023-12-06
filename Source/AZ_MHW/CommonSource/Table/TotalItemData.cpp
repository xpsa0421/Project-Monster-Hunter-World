#include "AZ_MHW/CommonSource/Table/TotalItemData.h"

void UTotalItemData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("name");
	if (value_ptr != nullptr)
	{
		name = **value_ptr;
	}
	value_ptr = data.Find("value");
	if (value_ptr != nullptr)
	{
		value = **value_ptr;
	}
	value_ptr = data.Find("warehouse_max");
	if (value_ptr != nullptr)
	{
		warehouse_max = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("pocket_max");
	if (value_ptr != nullptr)
	{
		pocket_max = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("init_count");
	if (value_ptr != nullptr)
	{
		init_count = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("type");
	if (value_ptr != nullptr)
	{
		type = **value_ptr;
	}
	value_ptr = data.Find("description");
	if (value_ptr != nullptr)
	{
		description = **value_ptr;
	}
	value_ptr = data.Find("kor_name");
	if (value_ptr != nullptr)
	{
		kor_name = **value_ptr;
	}
	value_ptr = data.Find("image_path");
	if (value_ptr != nullptr)
	{
		image_path = **value_ptr;
	}
	value_ptr = data.Find("color");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			color.Add(FCString::Atof(*str));
		}
	}
	value_ptr = data.Find("grade_image_path");
	if (value_ptr != nullptr)
	{
		grade_image_path = **value_ptr;
	}
}
