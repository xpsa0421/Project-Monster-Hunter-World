#include "AZ_MHW/CommonSource/Table/MonsterNonCombatActionData.h"

void UMonsterNonCombatActionData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("action_id");
	if (value_ptr != nullptr)
	{
		action_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("monster_id");
	if (value_ptr != nullptr)
	{
		monster_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("animation_name");
	if (value_ptr != nullptr)
	{
		animation_name = **value_ptr;
	}
	value_ptr = data.Find("montage_section_name");
	if (value_ptr != nullptr)
	{
		montage_section_name = **value_ptr;
	}
	value_ptr = data.Find("triggers");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			triggers.Add(str);
		}
	}
	value_ptr = data.Find("conditions");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			conditions.Add(str);
		}
	}
	value_ptr = data.Find("condition_min_health_ratio");
	if (value_ptr != nullptr)
	{
		condition_min_health_ratio = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("condition_max_health_ratio");
	if (value_ptr != nullptr)
	{
		condition_max_health_ratio = FCString::Atoi(**value_ptr);
	}
}
