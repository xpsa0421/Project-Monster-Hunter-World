// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Map/AZMapBase.h"

#include "AZ_MHW/Map/AZMapStatics.h"
#include "AZ_MHW/CommonSource/Table/DungeonData.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
#include "AZ_MHW/Util/AZUtility.h"

UAZMapBase::UAZMapBase()
{
	map_data_ = nullptr;
	dungeon_map_clear_ = INDEX_NONE;
}

UAZMapBase::~UAZMapBase()
{
	map_data_ = nullptr;
}

void UAZMapBase::OnCreate(const UMapData* map_data)
{
	map_data_ = map_data;
	spawn_object_list_ = UAZMapStatics::GetMapSpawnObjectInfo(map_data->id);

	dungeon_map_clear_ = INDEX_NONE;
}

void UAZMapBase::OnStart()
{
}

void UAZMapBase::OnMapExit()
{
}

bool UAZMapBase::OnRequestMapOut()
{
	return true;
}

const FSpawnObjectInfo* UAZMapBase::FindObjectByNPC(ENpcFuncType npc_type)
{
	// FIXME 나중에 추가하기
	/*const TArray<const FSpawnObjectInfo*> objects = FindObjects([npc_type](const FSpawnObjectInfo* item)
		{
			if (const UNpcData* npc_data = GetTable(NpcData, item->object_index))
			{
				ENpcFuncType npc_func_type = static_cast<ENpcFuncType>(npc_data->npc_func_type);
				return npc_func_type == npc_type;
			}
			return false;
		});*/
	return /*objects.IsValidIndex(0) ? objects[0] :*/ nullptr;
}

EMapType UAZMapBase::GetMapType() const
{
	if (map_data_ != nullptr)
	{
		EMapType type = UAZUtility::StringToEnum<EMapType>(map_data_->map_type);
		return type;
	}
	return EMapType::None;
}

int32 UAZMapBase::GetMapIndex() const
{
	if (map_data_)
	{
		return map_data_->id;
	}
	return INDEX_NONE;
}

void UAZDungeon::OnCreate(const UMapData* map_data)
{
	Super::OnCreate(map_data);
}

UAZDungeon* UAZDungeon::CreateDungeon(int32 map_index)
{
	if (auto map_data = GetTableByIndex(UMapData, map_index))
	{
		EMapType map_type = UAZUtility::StringToEnum<EMapType>(map_data->map_type);
		if (map_type == EMapType::Dungeon)
		{
			auto dungeon = NewObject<UAZDungeon>();
			dungeon->SetDungeonInfo(map_index);
		}
	}
	return nullptr;
}

const UDungeonData* UAZDungeon::GetCurrentMainGoalInfo() const
{
	return nullptr;
}

const UDungeonData* UAZDungeon::GetMainGoalInfo() const
{
	return nullptr;
}

int32 UAZDungeon::GetTargetObjectSerial()
{
	return int32();
}

void UAZDungeon::SetDungeonInfo(int32 map_index)
{
	dungeon_goal_list = GetGameSingleton()->table_mgr->GetData<UDungeonData>([map_index](const UDungeonData* check) {
		if (check->map_index == map_index)
		{
			return true;
		}
		return false;
		});
}
