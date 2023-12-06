// Fill out your copyright notice in the Description page of Project Settings.


#include "AZInventoryManager.h"
#include "AZTableMgr.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "Util/AZUtility.h"

void UAZInventoryManager::Init()
{
	instance_ = UAZGameSingleton::instance();
	
	ResetMgr();
	SetMaxCount();
	GetTableData();
	CreateStartItem();
	SetPotionSlot();
	SetBottleSlot();
}

void UAZInventoryManager::ResetMgr()
{
	potion_pocket_.Empty();
	potion_warehouse_.Empty();
	bottle_pocket_.Empty();
	bottle_warehouse_.Empty();
	weapon_warehouse_.Empty();
	armor_warehouse_.Empty();
}

void UAZInventoryManager::SetMaxCount()
{
	potion_warehouse_max_count = 50;
	potion_pocket_max_count = 15;
	bottle_warehouse_max_count = 20;
	bottle_pocket_max_count = 10;
}

void UAZInventoryManager::GetTableData()
{
	for(auto total_item : instance_->table_mgr->total_item_array_)
	{
		FTotalItemDataStruct total_data;
		total_data.id = total_item->id;
		total_data.name = total_item->name;
		total_data.warehouse_max = total_item->warehouse_max;
		total_data.pocket_max = total_item->pocket_max;
		total_data.init_count = total_item->init_count;
		FString type = total_item->type;
		total_data.type = UAZUtility::StringToEnum<EItemType>(type);
		total_data_map_.Emplace(total_data.id, total_data);
	}
	
	for(auto potion_data : instance_->table_mgr->potion_item_array_)
	{
		FPotionDataStruct potion_struct;
		potion_struct.id = potion_data->id;
		potion_struct.is_buff = potion_data->is_buff;
		potion_struct.is_usable = potion_data->usable;
		potion_data_map_.Emplace(potion_struct.id, potion_struct);
		
	}
	for(auto bottle_data : instance_->table_mgr->bottle_array_)
	{
		FBottleDataStruct bottle_struct;
		bottle_struct.id = bottle_data->id;
		bottle_struct.is_buff = bottle_data->is_buff;
		bottle_data_map_.Emplace(bottle_struct.id, bottle_struct);
	}
	for(auto buff_data : instance_->table_mgr->buff_array_)
	{
		FBuffDataStruct buff_struct;
		buff_struct.id = buff_data->id;
		buff_struct.effect = buff_data->effect;
		buff_struct.target = UAZUtility::StringToEnum<EItemTarget>(buff_data->target);
		buff_struct.calc = UAZUtility::StringToEnum<ECalculation>(buff_data->calculation);
		buff_data_map_.Emplace(buff_struct.id,buff_struct);
	}
	for(auto melee_weapon : instance_->table_mgr->melee_weapon_array_)
	{
		FMeleeWeaponDataStruct m_weapon_struct;
		m_weapon_struct.id = melee_weapon->id;
		m_weapon_struct.name = melee_weapon->name;
		m_weapon_struct.type = UAZUtility::StringToEnum<EWeaponType>(melee_weapon->type);
		m_weapon_struct.damage = melee_weapon->damage;
		m_weapon_struct.hone = melee_weapon->hone;
		m_weapon_struct.critical = melee_weapon->critical;
		melee_weapon_data_map_.Emplace(m_weapon_struct.id, m_weapon_struct);
	
	}
	for(auto armor : instance_->table_mgr->armor_array_)
	{
		FArmorDataStruct armor_struct;
		armor_struct.id = armor->id;
		armor_struct.type = UAZUtility::StringToEnum<EArmorType>(armor->type);
		armor_struct.defense = armor->defense;
		armor_data_map_.Emplace(armor_struct.id, armor_struct);
	}

}

void UAZInventoryManager::CreateStartItem()
{
	for(auto item : total_data_map_)
	{
		if(item.Value.type == EItemType::potion)
		{
			FPotionInfo info;
			info.item_key = item.Value.id;
			info.item_name = item.Value.name;
			info.item_type = item.Value.type;
			info.item_count = item.Value.init_count;
			FPotionDataStruct* potion_data =  potion_data_map_.Find(item.Value.id);
			info.usable = potion_data->is_usable;
			if(potion_data->is_buff == true)
			{
				FBuffDataStruct* buff_data = buff_data_map_.Find(info.item_key);
				if(buff_data == nullptr)
				{
					return;
				}
				info.item_effect = buff_data->effect;
				info.calc_type = buff_data->calc;
				info.target = buff_data->target;
			}
			AddPocketPotion(info);
			AddWarehousePotion(info);
		}
		else if(item.Value.type == EItemType::ammo)
		{
			FAmmoInfo info;
			info.item_key = item.Value.id;
			info.item_name = item.Value.name;
			info.item_type = item.Value.type;
			info.item_count = item.Value.init_count;
			FBottleDataStruct* bottle_data = bottle_data_map_.Find(info.item_key);
			if(bottle_data->is_buff == true)
			{
				FBuffDataStruct* buff_data = buff_data_map_.Find(info.item_key);
				info.item_effect = buff_data->effect;
				info.calc_type = buff_data->calc;
				info.effect_type = buff_data->target;
			}
			AddPocketBottle(info);
		}
		else if(item.Value.type == EItemType::weapon)
		{
			FWeaponInfo info;
			info.item_key = item.Value.id;
			info.item_name = item.Value.name;
			info.item_type = item.Value.type;
			if(melee_weapon_data_map_.Contains(info.item_key))
			{
				FMeleeWeaponDataStruct* m_weapon = melee_weapon_data_map_.Find(info.item_key);
				info.damage = m_weapon->damage;
				info.hone = m_weapon->hone;
				info.is_equip = false;
				info.weapon_type = m_weapon->type;
				
			}
			AddWarehouseWeapon(info);
		}
		else if(item.Value.type == EItemType::armor)
		{
			FArmorInfo info;
			info.item_key = item.Value.id;
			info.item_name = item.Value.name;
			info.item_type = item.Value.type;
			if(armor_data_map_.Contains(info.item_key))
			{
				FArmorDataStruct* armor = armor_data_map_.Find(info.item_key);
				info.is_equip = false;
				info.defense = armor->defense;
				info.armor_type = armor->type;
			}
			AddWarehouseArmor(info);
		}
	}
}
int32 UAZInventoryManager::GetInventoryCurrCount(EItemType item_type, EStorageType type)
{
	int32 count = 0;
	switch (item_type)
	{ 
	case EItemType::potion:
		if (type == EStorageType::Warehouse)
		{
			count = potion_warehouse_.Num();
		}
		else
		{
			count = potion_pocket_.Num();
		}
		break;
	case EItemType::ammo:
		if (type == EStorageType::Warehouse)
		{
			count = bottle_warehouse_.Num();
		}
		else
		{
			count = bottle_pocket_.Num();
		}
		break;
	}
	return count;
}

bool UAZInventoryManager::IsWarehouseFull(EItemType type)
{
	int32 curr_count = 0;
	int32 max_count = 0;
	int32 empty = 0;
	switch (type)
	{
	case EItemType::potion:
		curr_count = GetInventoryCurrCount(EItemType::potion, EStorageType::Warehouse);
		max_count = potion_warehouse_max_count;
		break;
	case EItemType::ammo:
		curr_count = GetInventoryCurrCount(EItemType::ammo, EStorageType::Warehouse);
		max_count = bottle_warehouse_max_count;
		break;
	}
	empty = max_count - curr_count;

	if (empty <= 0)
	{
		return true;
	}
	return false;
}

bool UAZInventoryManager::IsPocketFull(EItemType type)
{
	int32 curr_count = 0;
	int32 max_count = 0;
	int32 empty = 0;
	switch (type)
	{
	case EItemType::potion:
		curr_count = GetInventoryCurrCount(EItemType::potion, EStorageType::Pocket);
		max_count = potion_pocket_max_count;
		break;
	case EItemType::ammo:
		curr_count = GetInventoryCurrCount(EItemType::ammo, EStorageType::Pocket);
		max_count = bottle_pocket_max_count;
		break;
	}
	empty = max_count - curr_count;
	if (empty <= 0)
	{
		return true;
	}
	return false;
}

void UAZInventoryManager::SortWarehouse()
{
	potion_warehouse_.KeySort([](int32 left, int32 right)
	{
		return left < right;
	});
	bottle_warehouse_.KeySort([](int32 left, int32 right)
	{
		return left<right;
	});
}

void UAZInventoryManager::SortPocket()
{
	potion_pocket_.KeySort([](int32 left, int32 right)
	{
		return left<right;
	});
	bottle_pocket_.KeySort([](int32 left, int32 right)
	{
		return left<right;
	});
}


void UAZInventoryManager::RemoveItem(int32 item_key, EItemType item_type, EStorageType type)
{
	switch (item_type)
	{
	case EItemType::potion:
		if(type == EStorageType::Warehouse)
		{
			potion_warehouse_.Remove(item_key);
		}
		else if(type == EStorageType::Pocket)
		{
			potion_pocket_.Remove(item_key);
		}
		break;
	case EItemType::ammo:
		if(type == EStorageType::Warehouse)
		{
			bottle_warehouse_.Remove(item_key);
		}
		else if(type == EStorageType::Pocket)
		{
			bottle_pocket_.Remove(item_key);
		}
		break;
	case EItemType::weapon:
		weapon_warehouse_.Remove(item_key);
		break;
	case EItemType::armor:
		armor_warehouse_.Remove(item_key);
		break;
	}
}

TArray<UAZPotionItem*> UAZInventoryManager::GetPotionSlot()
{
	return potion_slot_;
}


TArray<UAZAmmoItem*> UAZInventoryManager::GetBottleSlot()
{
	return bottle_slot_;
}

TArray<UAZWeaponItem*> UAZInventoryManager::GetWeaponSlot()
{
	return weapon_slot_;
}


TArray<UAZArmorItem*> UAZInventoryManager::GetArmorSlot()
{
	return total_armor_slot_;
}

bool UAZInventoryManager::AddWarehousePotion(FPotionInfo& info)
{
	bool is_full = IsWarehouseFull(EItemType::potion);
	if (is_full == true)
	{
		return false;
	}

	UAZPotionItem** potion = potion_warehouse_.Find(info.item_key);

	if(potion!= nullptr)
	{
		(*potion)->IncreaseCount(info.item_count);
	}
	else
	{
		info.storage_type = EStorageType::Warehouse;
		potion_warehouse_.Emplace(info.item_key,CreatePotion(info));
	}
	return true;
}

bool UAZInventoryManager::AddPocketPotion( FPotionInfo& info)
{
	bool is_full = IsPocketFull(EItemType::potion);
	if (is_full == true)
	{
		return false;
	}
	UAZPotionItem** potion = potion_pocket_.Find(info.item_key);
	if(potion != nullptr)
	{
		(*potion)->IncreaseCount(info.item_count);
	}
	else
	{
		info.storage_type = EStorageType::Pocket;
		potion_pocket_.Emplace(info.item_key, CreatePotion(info));
	}
	
	return true;
}

UAZPotionItem* UAZInventoryManager::CreatePotion(FPotionInfo& info)
{
	UAZPotionItem* potion = NewObject<UAZPotionItem>();
	potion->InitItem(info);
	return potion;
}


bool UAZInventoryManager::ChangePotionStorage(int32 item_key, EStorageType type, int32 move_count)
{
	EStorageType pre_state = type;
	FPotionInfo new_info;
	UAZPotionItem** potion = nullptr;
	if(pre_state == EStorageType::Warehouse)
	{
		potion =  potion_warehouse_.Find(item_key);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		potion = potion_pocket_.Find(item_key);
	}
	
	if(potion == nullptr)
	{
		return false;
	}
	FPotionInfo info = (*potion)->GetItemInfo();
	info.item_count -= move_count;
	if(info.item_count<=0)
	{
		RemoveItem(info.item_key,info.item_type, info.storage_type);
	}
	new_info = info;
	new_info.item_count = move_count;
	if(pre_state == EStorageType::Warehouse)
	{
		AddPocketPotion(new_info);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		AddWarehousePotion(new_info);
	}
	return true;
}

void UAZInventoryManager::SetPotionSlot()
{
	for(auto item : potion_pocket_)
	{
		potion_slot_.Emplace(item.Value);
	}
}

FBuffDataStruct UAZInventoryManager::UsePotion(int32 item_index)
{
	UAZPotionItem* potion = potion_slot_[item_index];
	FBuffDataStruct buff;
	potion->DecreaseCount();
	if(potion->GetItemCount() == 0)
	{
		potion_pocket_.Remove(potion->GetItemInfo().item_key);
	}
	buff = *buff_data_map_.Find(potion->GetItemInfo().item_key);
	return buff;
}

bool UAZInventoryManager::AddWarehouseBottle(FAmmoInfo& info)
{
	bool is_full = IsWarehouseFull(EItemType::ammo);
	if(is_full == true)
	{
		return false;
	}
	UAZAmmoItem** bottle = bottle_warehouse_.Find(info.item_key);

	if(bottle != nullptr)
	{
		(*bottle)->IncreaseCount();
	}
	else
	{
		info.storage_type = EStorageType::Warehouse;
		bottle_warehouse_.Emplace(info.item_key, CreateBottle(info));
	}
	return true;
}

bool UAZInventoryManager::AddPocketBottle(FAmmoInfo& info)
{
	bool is_full = IsPocketFull(EItemType::ammo);
	if(is_full == true)
	{
		return false;
	}
	UAZAmmoItem** bottle = bottle_pocket_.Find(info.item_key);
	if(bottle != nullptr)
	{
		(*bottle)->IncreaseCount();
	}
	else
	{
		info.storage_type = EStorageType::Pocket;
		bottle_pocket_.Emplace(info.item_key,CreateBottle(info));
	}
	return true;
}

UAZAmmoItem* UAZInventoryManager::CreateBottle(FAmmoInfo& info)
{
	UAZAmmoItem* bottle = NewObject<UAZAmmoItem>();
	bottle->InitItem(info);
	return bottle;
}

UAZWeaponItem* UAZInventoryManager::CreateWeapon(FWeaponInfo& info)
{
	UAZWeaponItem* weapon = NewObject<UAZWeaponItem>();
	weapon->InitItem(info);
	return weapon;
}

bool UAZInventoryManager::AddWarehouseWeapon(FWeaponInfo& info)
{
	UAZWeaponItem** weapon = weapon_warehouse_.Find(info.item_key);

	if(weapon != nullptr)
	{
		return false;
	}
	else
	{
		weapon_warehouse_.Emplace(info.item_key, CreateWeapon(info));
	}

	return true;
}

void UAZInventoryManager::ChangeWeaponEquipState(int32 item_key)
{
	UAZWeaponItem** weapon = weapon_warehouse_.Find(item_key);
	if(weapon == nullptr)
	{
		return;
	}
	if((*weapon)->GetEquipState() == false)
	{
		(*weapon)->EquipStateChange(true);
	}
	else if((*weapon)->GetEquipState() == true)
	{
		(*weapon)->EquipStateChange(false);
	}
}


void UAZInventoryManager::SetWeaponSlot()
{
	for(auto weapon : weapon_warehouse_)
	{
		weapon_slot_.Emplace(weapon.Value);
	}
}

FWeaponInfo UAZInventoryManager::GetEquipWeaponInfo()
{
	FWeaponInfo info;
	for(auto weapon : weapon_warehouse_)
	{
		if(weapon.Value->GetWeaponInfo().is_equip == true)
		{
			info = weapon.Value->GetWeaponInfo();
		}
	}
	return info;
}

int32 UAZInventoryManager::GetEquipWeaponKey()
{

	FWeaponInfo info;
	for(auto weapon : weapon_warehouse_)
	{
		if(weapon.Value->GetWeaponInfo().is_equip == true)
		{
			info = weapon.Value->GetWeaponInfo();
		}
	}
	return info.item_key;
}

UAZWeaponItem* UAZInventoryManager::GetEquipWeapon()
{
	UAZWeaponItem* equip_weapon = nullptr;
	for(auto weapon : weapon_warehouse_)
	{
		if(weapon.Value->GetEquipState() == true)
		{
			equip_weapon = weapon.Value;
		}
	}
	return equip_weapon;
}

UAZArmorItem* UAZInventoryManager::CreateArmor(FArmorInfo& info)
{
	UAZArmorItem* armor = NewObject<UAZArmorItem>();
	armor->InitItem(info);
	return armor;
}

UAZArmorItem* UAZInventoryManager::GetEquipArmorToType(int32 armor_index)
{
	UAZArmorItem* armor = nullptr;
	EArmorType armor_type_ = static_cast<EArmorType>(armor_index);
	armor = FindEquipArmor(armor_type_);
	
	return armor;
}

UAZArmorItem* UAZInventoryManager::FindEquipArmor(EArmorType type)
{
	UAZArmorItem* item = nullptr;
	for(auto armor : armor_warehouse_)
	{
		if(armor.Value->GetItemInfo().armor_type == type)
		{
			if(armor.Value->GetItemInfo().is_equip == true)
			{
				item = armor.Value;
			}
		}
	}
	return item;
}

bool UAZInventoryManager::AddWarehouseArmor(FArmorInfo& info)
{
	UAZArmorItem** armor = armor_warehouse_.Find(info.item_key);

	if(armor != nullptr)
	{
		return false;
	}
	armor_warehouse_.Emplace(info.item_key, CreateArmor(info));
	return true;
}

void UAZInventoryManager::ChangeArmorEquipState(int32 item_key)
{
	UAZArmorItem** armor = armor_warehouse_.Find(item_key);
	if(armor == nullptr)
	{
		return;
	}
	if((*armor)->GetEquipState() == false)
	{
		(*armor)->EquipStateChange(true);
	}
	else if((*armor)->GetEquipState() == true)
	{
		(*armor)->EquipStateChange(false);
	}
}


void UAZInventoryManager::SetArmorSlot()
{
	for(auto armor : armor_warehouse_)
	{
		total_armor_slot_.Emplace(armor.Value);
	}
}

int32 UAZInventoryManager::GetEquipArmorKey(EArmorType type)
{
	auto armor = FindEquipArmor(type);
	if(armor == nullptr)
	{
		return 0;
	}
	return armor->GetItemInfo().item_key;
}

bool UAZInventoryManager::ChangeBottleStorage(int32 item_key, EStorageType type, int32 move_count)
{
	EStorageType pre_state = type;
	FAmmoInfo new_info;
	UAZAmmoItem** bottle = nullptr;
	if(pre_state == EStorageType::Warehouse)
	{
		bottle = bottle_warehouse_.Find(item_key);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		bottle = bottle_pocket_.Find(item_key);
	}
	if(bottle == nullptr)
	{
		return false;
	}
	FAmmoInfo info = (*bottle)->GetItemInfo();
	new_info = info;
	(*bottle)->DecreaseCount(move_count);
	if((*bottle)->GetItemCount() == 0)
	{
		RemoveItem(info.item_key, info.item_type, info.storage_type);
	}
	new_info.item_count = move_count;
	if(pre_state ==EStorageType::Warehouse)
	{
		AddPocketBottle(new_info);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		AddWarehouseBottle(new_info);
	}
	return true;
}

void UAZInventoryManager::SetBottleSlot()
{
	for(auto item : bottle_pocket_)
	{
		bottle_slot_.Emplace(item.Value);
	}
}

FBuffDataStruct UAZInventoryManager::UseBottle(int32 index)
{
	UAZAmmoItem* item =bottle_slot_[index];
	FBuffDataStruct buff;

	if(item->GetItemInfo().item_name != "normal_bottle")
	{
		item->DecreaseCount();
	}
	
	if(item->GetItemCount() == 0)
	{
		bottle_pocket_.Remove(item->GetItemKey());
	}
	buff = *buff_data_map_.Find(item->GetItemKey());
	UAZAmmoItem** temp =  bottle_pocket_.Find(buff.id);
	if(temp != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  FString::Printf(TEXT("item : %s , count : %d"),
			*((*temp)->GetItemInfo().item_name),(*temp)->GetItemInfo().item_count));

	}
	return buff;
}

