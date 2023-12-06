#include "AZ_MHW/CommonSource/Table/CommandData.h"

void UCommandData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("StateType(이름)");
	if (value_ptr != nullptr)
	{
	}
	value_ptr = data.Find("SectionName");
	if (value_ptr != nullptr)
	{
	}
	value_ptr = data.Find("InputMask");
	if (value_ptr != nullptr)
	{
	}
	value_ptr = data.Find("CommandSectionName");
	if (value_ptr != nullptr)
	{
	}
}
