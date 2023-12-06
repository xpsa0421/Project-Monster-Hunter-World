#include "AZ_MHW/CommonSource/Table/ItemBuffData.h"

void UItemBuffData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("target");
	if (value_ptr != nullptr)
	{
		target = **value_ptr;
	}
	value_ptr = data.Find("effect");
	if (value_ptr != nullptr)
	{
		effect = FCString::Atof(**value_ptr);
	}
	value_ptr = data.Find("duration_time");
	if (value_ptr != nullptr)
	{
		duration_time = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("particle_path");
	if (value_ptr != nullptr)
	{
		particle_path = **value_ptr;
	}
	value_ptr = data.Find("calculation");
	if (value_ptr != nullptr)
	{
		calculation = **value_ptr;
	}
}
