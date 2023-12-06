// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/Table/MapData.h"
#include "AZ_MHW/CommonSource/Define/MapStruct.h"
#include "AZ_MHW/CommonSource/Define/ObjectStruct.h"
#include "AZMapBase.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZMapBase : public UObject
{
	GENERATED_BODY()
public:
	UAZMapBase();
	~UAZMapBase();
	virtual void OnCreate(const UMapData* map_data);
	virtual void OnStart();
	virtual void OnMapExit();
	virtual bool OnRequestMapOut();
	virtual void OnGameMsg(struct FAZGameMsg* new_msg) {}

	// Area의 접근이 없음
	//virtual void OnEnterArea(...)

	// 상점 열기 작업이 필요
	const FSpawnObjectInfo* FindObjectByNPC(ENpcFuncType npc_type);

	template <typename predicate>
	const TArray<const FSpawnObjectInfo*> FindObjects(predicate pred)
	{
		TArray<const FSpawnObjectInfo*> item_array;
		for (int32 i = 0; i < spawn_object_list_.Num(); ++i)
		{
			if (pred(spawn_object_list_[i]))
			{
				item_array.Add(&spawn_object_list_[i]);
			}
		}
		return item_array;
	}

	EMapType GetMapType() const;
	int32 GetMapIndex() const;
	const UMapData* GetMapData()
	{
		return map_data_;
	}

protected:
	UPROPERTY() const UMapData* map_data_;
	TArray<FSpawnObjectInfo> spawn_object_list_;
	int32 dungeon_map_clear_;
};

// 파티던전
UCLASS()
class AZ_MHW_API UAZDungeon : public UAZMapBase
{
	GENERATED_BODY()

public:
	virtual void OnCreate(const UMapData* map_data) override;

public:
	static UAZDungeon* CreateDungeon(int32 map_index);
	const class UDungeonData* GetCurrentMainGoalInfo() const;
	const class UDungeonData* GetMainGoalInfo() const;
	int32 GetTargetObjectSerial();
	void SetDungeonInfo(int32 map_index);

private:
	TArray<const class UDungeonData*> dungeon_goal_list;
	TSet<int32> defeat_spawn_indexes_;
	TSet<int32> clear_area_indexes_;

public:
	UPROPERTY() int32 current_observed_target_object_serial;
};
