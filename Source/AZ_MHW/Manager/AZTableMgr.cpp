// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZTableMgr.h"

#include "AZ_MHW/CommonSource/Table/ConstantData.h"
#include "AZ_MHW/CommonSource/Table/TestData.h"

void UAZTableMgr::LoadComplete()
{
	/*const UTestData* test1 = GetData<UTestData>(1);
	TArray<const UTestData*> test_results;*/
	/*total_item_array = GetData<UTOTAL_ITEM_TABLE>([](const UTOTAL_ITEM_TABLE* check) {
		if (check->count != 0)
		{
			return true;
		}
		return false;
		});*/
	LoadTotalItemTable();
	LoadPotionTable();
	LoadBuffTable();
	LoadBottleTable();
	LoadWeaponTable();
	LoadArmorTable();

}

void UAZTableMgr::LoadTotalItemTable()
{
	total_item_array_ = GetData<UTotalItemData>([](const UTotalItemData* total_item)
	{
	if (total_item->type != "hair")
	{
		return true;
	}
	return false;
	});
}

void UAZTableMgr::LoadPotionTable()
{
	potion_item_array_ = GetData<UPotionData>([](const UPotionData* consume_item)
	{
		if(consume_item->is_buff == true)
		{
			return true;	
		}
		return false;
	});

}

void UAZTableMgr::LoadBuffTable()
{
	buff_array_ =GetData<UItemBuffData>([](const UItemBuffData* buff)
	{
		return true;
	});
}

void UAZTableMgr::LoadBottleTable()
{
	bottle_array_ = GetData<UBottleData>([](const UBottleData* bottle)
	{
		return true;
	});
}
void UAZTableMgr::LoadWeaponTable()
{
	melee_weapon_array_ = GetData<UMeleeWeaponData>([](const UMeleeWeaponData* m_weapon)
	{
		return true;
	});
}

void UAZTableMgr::LoadArmorTable()
{
	armor_array_ = GetData<UArmorData>([](const UArmorData* armor)
	{
		return true;
	});
}