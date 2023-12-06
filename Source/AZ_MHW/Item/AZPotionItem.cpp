// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPotionItem.h"

void UAZPotionItem::InitItem(FPotionInfo& new_info)
{
	info_ = new_info;
}

void UAZPotionItem::SetCount(int32 count)
{
	info_.item_count = count;
}

void UAZPotionItem::IncreaseCount(int32 count)
{
	info_.item_count = info_.item_count + count;
}

void UAZPotionItem::DecreaseCount(int32 count)
{
	info_.item_count = info_.item_count - count;
	if(info_.item_count <= 0)
	{
		info_.item_count = 0;
	}
}

FPotionInfo UAZPotionItem::GetItemInfo() const
{
	return info_;
}

int32 UAZPotionItem::GetItemCount()
{
	return info_.item_count;
}

int32 UAZPotionItem::GetItemKey()
{
	return info_.item_key;
}

EStorageType UAZPotionItem::GetItemStorageType()
{
	return info_.storage_type;
}
