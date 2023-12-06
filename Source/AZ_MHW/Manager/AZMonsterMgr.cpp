#include "AZMonsterMgr.h"
#include <AZ_MHW/CommonSource/Table/BossData.h>
#include <AZ_MHW/CommonSource/Table/MonsterData.h>
#include <AZ_MHW/CommonSource/Table/MonsterNonCombatActionData.h>
#include <AZ_MHW/CommonSource/Table/MonsterCombatActionData.h>
#include "AZTableMgr.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "BehaviorTree/BehaviorTree.h"

void UAZMonsterMgr::Init()
{
	if (!LoadBehaviorTreeAssets())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMgr] Behavior tree assets are not found!"));
	}
	if (!ConvertMonsterTable())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMgr] Monster Table is not found!"));
	}
	if (!ConvertBossTable())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMgr] Boss Table is not found!"));
	}
	if (!ConvertMonsterNonCombatActionTable())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMgr] Monster NonCombat Action Table is not found!"));
	}
	if (!ConvertMonsterCombatActionTable())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMgr] Monster Combat Action Table is not found!"));
	}
}

bool UAZMonsterMgr::LoadBehaviorTreeAssets()
{
	// Load Asset Registry Module
	const FAssetRegistryModule& asset_registry_module = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// Retrieve behavior tree assets from the ai directory
	TArray<FAssetData> assets;
	FARFilter filter;
	filter.ClassPaths.Add(UBehaviorTree::StaticClass()->GetClassPathName());
	filter.PackagePaths.Add(TEXT("/Game/AZ/Monsters/Common/AI"));	
	asset_registry_module.Get().GetAssets(filter, assets);
	
	// Add to map
	for (const FAssetData& asset_data : assets)
	{
		// Get asset name
		FString filename_part, path_part, extension_part;
		FPaths::Split(asset_data.GetSoftObjectPath().ToString(), path_part, filename_part,extension_part);

		if (auto bt_asset = Cast<UBehaviorTree>(asset_data.GetAsset()))
		{
			behavior_tree_map_.Add(FName(filename_part.Mid(3)), bt_asset);
		}
	}

	// Unload Asset Registry Module
	FModuleManager::Get().UnloadModule("AssetRegistry");

	// Check if assets are loaded successfully
	if (behavior_tree_map_.IsEmpty()) return false;
	else return true;
}

bool UAZMonsterMgr::ConvertMonsterTable()
{
	auto monster_table = UAZGameSingleton::instance()->table_mgr->GetData<UMonsterData>();
	if (monster_table.IsEmpty()) return false;
	
	for (const auto monster_data : monster_table)
	{
		FMonsterInfo monster_info;
		monster_info.monster_id					= monster_data->monster_id;
		monster_info.behavior_type				= UAZUtility::StringToEnum<EMonsterBehaviorType>(monster_data->behavior_type);
		monster_info.base_hp					= monster_data->base_hp;
		monster_info.sight_configs				= FMonsterSightConfigs(monster_data->sight_radius,
													monster_data->sight_lose_radius, monster_data->sight_fov,
													UAZUtility::MillisecondsToSeconds(monster_data->sight_max_age),
													monster_data->sight_auto_success_range);
		monster_info.patrol_range				= monster_data->patrol_range;
		monster_info.patrol_delay				= UAZUtility::MillisecondsToSeconds(monster_data->patrol_delay);
		monster_info.percept_radius				= monster_data->percept_radius;
		monster_info.name						= FName(monster_data->name);

		monster_info_map_.Add(monster_info.monster_id, monster_info);
	}
	return true;
}

bool UAZMonsterMgr::ConvertBossTable()
{
	auto boss_table = UAZGameSingleton::instance()->table_mgr->GetData<UBossData>();
	if (boss_table.IsEmpty()) return false;
	
	for (const UBossData* boss_data : boss_table)
	{
		FBossInfo boss_info;
		boss_info.boss_id							= boss_data->boss_id;
		boss_info.monster_id						= boss_data->monster_id;
		boss_info.rank								= UAZUtility::StringToEnum<EBossRank>(boss_data->rank);
		boss_info.base_stamina					= boss_data->base_stamina;
		boss_info.tired_duration					= UAZUtility::MillisecondsToSeconds(boss_data->tired_duration);
		boss_info.has_transition_animation			= boss_data->has_transition_animation;
		boss_info.weakness_stats					= FBossWeaknessStats(boss_data->weakness_head, boss_data->weakness_neck,
														boss_data->weakness_body, boss_data->weakness_tail,
														boss_data->weakness_wing,boss_data->weakness_leg);
		boss_info.head_state						= FBossBodyPartState(boss_data->damage_head);
		boss_info.body_state						= FBossBodyPartState(boss_data->damage_body);
		boss_info.wing_state						= FBossBodyPartState(boss_data->damage_wing);
		boss_info.tail_state						= FBossBodyPartState(boss_data->damage_tail);
		boss_info.leg_state							= FBossBodyPartState(boss_data->damage_leg);
		boss_info.stunnable_parts					= UAZUtility::StringArrToEnumArr<EMonsterBodyPart>(boss_data->stunnable_parts);
		boss_info.rage_stats						= FBossRageStats(boss_data->rage_required_damage, boss_data->rage_duration,
														boss_data->rage_outgoing_damage_multiplier,	boss_data->rage_incoming_damage_multiplier);
		boss_info.tenderised_damage_multiplier		= UAZUtility::PerTenThousandToPerOne(boss_data->tenderised_damage_multiplier);
		boss_info.escape_stats						= FBossEscapeStats(boss_data->num_allowed_escapes, boss_data->escape_health_ratios);

		boss_info_map_.Add(boss_info.monster_id * 1000 + (int)boss_info.rank, boss_info);
	}
	return true;
}

bool UAZMonsterMgr::ConvertMonsterNonCombatActionTable()
{
	auto monster_noncombat_table = UAZGameSingleton::instance()->table_mgr->GetData<UMonsterNonCombatActionData>();
	if (monster_noncombat_table.IsEmpty()) return false;
	
	for (const auto noncombat_action_data : monster_noncombat_table)
	{
		FMonsterNonCombatActionInfo noncombat_action_info;
		noncombat_action_info.action_id						= noncombat_action_data->action_id;
		noncombat_action_info.monster_id					= noncombat_action_data->monster_id;
		noncombat_action_info.animation_name				= FName(noncombat_action_data->animation_name);
		noncombat_action_info.montage_section_name			= FName(noncombat_action_data->montage_section_name);
		noncombat_action_info.conditions					= UAZUtility::StringArrToBitMaskEnum<EMonsterActionConditionType>(noncombat_action_data->conditions);
		noncombat_action_info.condition_min_health_ratio	= UAZUtility::PerTenThousandToPerOne(noncombat_action_data->condition_min_health_ratio);
		noncombat_action_info.condition_max_health_ratio	= UAZUtility::PerTenThousandToPerOne(noncombat_action_data->condition_max_health_ratio);

		//monster_noncombat_action_info_map_.FindOrAdd(noncombat_action_info.monster_id).Add(noncombat_action_info);
		monster_noncombat_action_info_map_.Add(noncombat_action_info.monster_id, noncombat_action_info.monster_id * 1000 + noncombat_action_info.action_id);
		noncombat_action_info_map_.Add(noncombat_action_info.monster_id * 1000 + noncombat_action_info.action_id, noncombat_action_info);
	}
	return true;
}

bool UAZMonsterMgr::ConvertMonsterCombatActionTable()
{
	auto monster_combat_table = UAZGameSingleton::instance()->table_mgr->GetData<UMonsterCombatActionData>();
	if (monster_combat_table.IsEmpty()) return false;
	
	for (const auto combat_action_data : monster_combat_table)
	{
		// Add to action info map
		FMonsterCombatActionInfo combat_action_info;
		combat_action_info.action_id							= combat_action_data->action_id;
		combat_action_info.monster_id							= combat_action_data->monster_id;
		combat_action_info.animation_name						= FName(combat_action_data->animation_name);
		combat_action_info.montage_section_name					= FName(combat_action_data->montage_section_name);
		combat_action_info.triggers								= UAZUtility::StringArrToBitMaskEnum<EMonsterActionTriggerType>(combat_action_data->triggers);
		combat_action_info.conditions							= UAZUtility::StringArrToBitMaskEnum<EMonsterActionConditionType>(combat_action_data->conditions);
		combat_action_info.condition_min_distance_from_target	= combat_action_data->condition_min_distance_from_target;
		combat_action_info.condition_max_distance_from_target	= combat_action_data->condition_max_distance_from_target;
		combat_action_info.attack_delay							= UAZUtility::MillisecondsToSeconds(combat_action_data->attack_delay);
		
		//monster_combat_action_info_map_.FindOrAdd(combat_action_info.monster_id).Add(combat_action_info);
		monster_combat_action_info_map_.Add(combat_action_info.monster_id, combat_action_info.monster_id * 1000 + combat_action_info.action_id);
		combat_action_info_map_.Add(combat_action_info.monster_id * 1000 + combat_action_info.action_id, combat_action_info);

		// Add to attack info map
		FAttackInfo attack_info;
		attack_info.attack_effect = UAZUtility::StringToEnum<EAttackEffectType>(combat_action_data->effect);
		for (int i = 0; i < combat_action_data->damage_types.Num(); i++)
		{
			EDamageType damage_type = UAZUtility::StringToEnum<EDamageType>(combat_action_data->damage_types[i]);
			int32 damage_amount = combat_action_data->damage_amounts[i];

			if (damage_type == EDamageType::None) continue;
			attack_info.damage_array.Add(FDamageInfo(damage_type, damage_amount));
		}
		monster_attack_info_map_.Add(combat_action_info.action_id, attack_info);
	}
	return true;
}

FMonsterInfo* UAZMonsterMgr::GetMonsterInfo(const int32 monster_id)
{
	return monster_info_map_.Find(monster_id);
}

FBossInfo UAZMonsterMgr::GetBossInfo(const int32 monster_id, const EBossRank rank)
{
	return *boss_info_map_.Find(monster_id * 1000 + (int)rank);
}

TMap<int32, FMonsterNonCombatActionInfo> UAZMonsterMgr::GetMonsterNonCombatActionInfo(const int32 monster_id)
{
	TMap<int32, FMonsterNonCombatActionInfo> result;
	TArray<int32> values_for_key;
	monster_noncombat_action_info_map_.MultiFind(monster_id, values_for_key);
	for (auto& key : values_for_key)
	{
		auto value = noncombat_action_info_map_.Find(key);
		if (value == nullptr)
		{
			continue;
		}

		result.Add(value->action_id, *value);
	}
	return result;
}

TMap<int32, FMonsterCombatActionInfo> UAZMonsterMgr::GetMonsterCombatActionInfo(const int32 monster_id)
{
	TMap<int32, FMonsterCombatActionInfo> result;
	TArray<int32> values_for_key;
	monster_combat_action_info_map_.MultiFind(monster_id, values_for_key);
	for (auto& key : values_for_key)
	{
		auto value = combat_action_info_map_.Find(key);
		if (value == nullptr)
		{
			continue;
		}

		result.Add(value->action_id, *value);
	}
	return result;
}

FAttackInfo* UAZMonsterMgr::GetAttackInfo(const int32 action_id)
{
	return monster_attack_info_map_.Find(action_id);
}

UBehaviorTree* UAZMonsterMgr::GetBehaviorTree(FName filename)
{
	if (const auto behavior_tree = behavior_tree_map_.Find(filename))
	{
		return *behavior_tree;
	}
	else return nullptr;
}

FName UAZMonsterMgr::GetMonsterName(int32 monster_id)
{
	if (const auto monster_info = monster_info_map_.Find(monster_id))
	{
		return monster_info->name;
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterMgr][#%d] Monster is not found. Name cannot be retrieved."), monster_id);
		return NAME_None;
	}
}
