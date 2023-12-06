#include "AZ_MHW/CommonSource/Table/ModelData.h"

void UModelData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("model_type");
	if (value_ptr != nullptr)
	{
		model_type = **value_ptr;
	}
	value_ptr = data.Find("model_index");
	if (value_ptr != nullptr)
	{
		model_index = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("model_group_id");
	if (value_ptr != nullptr)
	{
		model_group_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("model_bp");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			model_bp.Add(str);
		}
	}
}
