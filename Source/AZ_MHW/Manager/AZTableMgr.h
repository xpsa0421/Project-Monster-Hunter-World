// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <typeinfo>
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/CommonSource/Table/TotalItemData.h"
#include "AZ_MHW/CommonSource/Table/PotionData.h"
#include "AZ_MHW/CommonSource/Table/ItemBuffData.h"
#include "AZ_MHW/CommonSource/Table/BottleData.h"
#include "AZ_MHW/CommonSource/Table/MeleeWeaponData.h"
#include "AZ_MHW/CommonSource/Table/ArmorData.h"

#include "AZTableMgr.generated.h"


#define GetTableByIndex(TableType, index) GetGameSingleton()->table_mgr->GetData<TableType>(index)
#define GetTable(TableType) GetGameSingleton()->table_mgr->GetData<TableType>();



UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZTableMgr : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY() TSet<const UObject*> map_set_;
	TMap<uint32, TMap<int32, const UObject*>> map_table_;

public:
	//item table map
	UPROPERTY() TArray<const UTotalItemData*> total_item_array_;
	UPROPERTY() TArray<const UPotionData*> potion_item_array_;
	UPROPERTY() TArray<const UItemBuffData*> buff_array_;
	UPROPERTY() TArray<const UBottleData*> bottle_array_;
	UPROPERTY() TArray<const UMeleeWeaponData*> melee_weapon_array_;
	UPROPERTY() TArray<const UArmorData*> armor_array_;
	
	template<class Table>
	bool _Load(FString file_name)
	{
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == true)
		{
			return false;
		}

		//map_table_.Add(file_name, TMap<int32, const UObject*>());
		FString table_path = FString::Printf(TEXT("%sRawContent/Csv/%s"), *FPaths::ProjectDir(), *file_name);

		TMap<int32, const UObject*> data_container;
		if (!FPaths::FileExists(table_path))
		{
			return false;
		}

		TArray<FString> lines;
		TArray<FString> fields;
		TArray<int> except_index;
		int row = 0;
		FFileHelper::LoadFileToStringArrayWithPredicate(lines, *table_path, [](const FString& Line) { return Line.Len() > 0; });
		for (FString& line : lines)
		{
			TArray<FString> cols;
			line.ParseIntoArray(cols, TEXT(","), true);
			if (row == 0)
			{
				for (int32 i = 0; i < cols.Num(); ++i)
				{
					if (cols[i].StartsWith("~"))
					{
						except_index.Add(i);
					}
					else
					{
						fields.Add(cols[i]);
					}
				}
			}

			if (row++ < 2)
			{
				continue;
			}

			int32 offset = 0;
			for (const int32 index : except_index)
			{
				cols.RemoveAt(index - offset);
				++offset;
			}

			TMap<FString, FString> col_values;
			for (int32 i = 0; i < fields.Num(); ++i)
			{
				col_values.Add(fields[i], cols[i]);
			}

			Table* table = NewObject<Table>();
			table->Load(col_values);

			int32 key = FCString::Atoi(*cols[0]);
			if (data_container.Contains(key) == true)
			{
				return false;
			}
			map_set_.Add(table);
			data_container.Add(key, table);
		}
		
		map_table_.Add(hash_code, data_container);
		return true;
	}

	//스크립트(대사 등) 테이블같은거 로드후 다사용하고 언로드 용도
	template<class Table>
	bool _UnLoad()
	{
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == false)
		{
			return false;
		}
		
		const auto table = map_table_.Find(hash_code);
		for(auto& tablePair : *table)
		{
			//UPROPERTY Pointer 전부 해제
			map_set_.Remove(tablePair.Value);
			//포인터자체를 nullptr로 교체해야하는가?
			tablePair.Value = nullptr;
		}
		//해당 테이블 전체 비우기
		table->Empty();
		//테이블 맵에서 제거
		map_table_.Remove(hash_code);
		return true;
	}
	template<class Table>
	TArray<const Table*> GetData(TFunctionRef<bool (const Table*)> Match)
	{
		TArray<const Table*> result;
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == false)
		{
			return result;
		}
		auto tables = map_table_.Find(hash_code);
		for (auto& table : *tables)
		{
			if (Match(Cast<Table>(table.Value)))
			{
				result.Add(Cast<Table>(table.Value));
			}
		}
		return result;
	}
	template<class Table>
	const Table* GetData(int32 key)
	{
		TArray<const Table*> result;
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == false)
		{
			return nullptr;
		}
		auto tables = map_table_.Find(hash_code);
		auto table = (*tables).Find(key);
		if (table == nullptr)
		{
			return nullptr;
		}
		return Cast<Table>(*table);
	}
	template<class Table>
	TArray<const Table*> GetData()
	{
		TArray<const Table*> result;
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == false)
		{
			return result;
		}
		auto tables = map_table_.Find(hash_code);
		for (auto& table : *tables)
		{
			result.Add(Cast<Table>(table.Value));
		}
		return result;
	}
	void LoadAll();
	void LoadComplete();
	void LoadTotalItemTable();
	void LoadPotionTable();
	void LoadBuffTable();
	void LoadBottleTable();
	void LoadWeaponTable();
	void LoadArmorTable();
};