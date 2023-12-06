#include "AZ_MHW/CommonSource/Table/ConstantData.h"

void UConstantData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test_int");
	if (value_ptr != nullptr)
	{
		test_int = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test_string");
	if (value_ptr != nullptr)
	{
		test_string = **value_ptr;
	}
}
