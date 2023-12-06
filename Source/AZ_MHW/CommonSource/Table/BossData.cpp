#include "AZ_MHW/CommonSource/Table/BossData.h"

void UBossData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("boss_id");
	if (value_ptr != nullptr)
	{
		boss_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("monster_id");
	if (value_ptr != nullptr)
	{
		monster_id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("rank");
	if (value_ptr != nullptr)
	{
		rank = **value_ptr;
	}
	value_ptr = data.Find("base_stamina");
	if (value_ptr != nullptr)
	{
		base_stamina = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("tired_duration");
	if (value_ptr != nullptr)
	{
		tired_duration = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("has_transition_animation");
	if (value_ptr != nullptr)
	{
		has_transition_animation = (*value_ptr).ToBool();
	}
	value_ptr = data.Find("num_allowed_escapes");
	if (value_ptr != nullptr)
	{
		num_allowed_escapes = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("escape_health_ratios");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			escape_health_ratios.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("weakness_head");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			weakness_head.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("weakness_neck");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			weakness_neck.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("weakness_body");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			weakness_body.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("weakness_tail");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			weakness_tail.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("weakness_wing");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			weakness_wing.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("weakness_leg");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			weakness_leg.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("damage_head");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			damage_head.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("damage_body");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			damage_body.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("damage_wing");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			damage_wing.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("damage_tail");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			damage_tail.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("damage_leg");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			damage_leg.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("stunnable_parts");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			stunnable_parts.Add(str);
		}
	}
	value_ptr = data.Find("rage_required_damage");
	if (value_ptr != nullptr)
	{
		rage_required_damage = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("rage_duration");
	if (value_ptr != nullptr)
	{
		rage_duration = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("rage_agility_multiplier");
	if (value_ptr != nullptr)
	{
		rage_agility_multiplier = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("rage_outgoing_damage_multiplier");
	if (value_ptr != nullptr)
	{
		rage_outgoing_damage_multiplier = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("rage_incoming_damage_multiplier");
	if (value_ptr != nullptr)
	{
		rage_incoming_damage_multiplier = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("tenderised_damage_multiplier");
	if (value_ptr != nullptr)
	{
		tenderised_damage_multiplier = FCString::Atoi(**value_ptr);
	}
}
