#include "AZ_MHW/CommonSource/Table/MonsterData.h"

void UMonsterData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("monster_id");
	if (value_ptr != nullptr)
	{
		monster_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("name");
	if (value_ptr != nullptr)
	{
		name = **value_ptr;
	}
	value_ptr = data.Find("behavior_type");
	if (value_ptr != nullptr)
	{
		behavior_type = **value_ptr;
	}
	value_ptr = data.Find("base_hp");
	if (value_ptr != nullptr)
	{
		base_hp = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("sight_radius");
	if (value_ptr != nullptr)
	{
		sight_radius = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("sight_lose_radius");
	if (value_ptr != nullptr)
	{
		sight_lose_radius = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("sight_fov");
	if (value_ptr != nullptr)
	{
		sight_fov = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("sight_max_age");
	if (value_ptr != nullptr)
	{
		sight_max_age = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("sight_auto_success_range");
	if (value_ptr != nullptr)
	{
		sight_auto_success_range = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("patrol_range");
	if (value_ptr != nullptr)
	{
		patrol_range = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("patrol_delay");
	if (value_ptr != nullptr)
	{
		patrol_delay = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("percept_radius");
	if (value_ptr != nullptr)
	{
		percept_radius = FCString::Atoi(**value_ptr);
	}
}
