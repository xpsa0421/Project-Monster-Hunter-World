// Fill out your copyright notice in the Description page of Project Settings.


#include "AZAmmoItem.h"


void UAZAmmoItem::InitItem(FAmmoInfo& item_info)
{
	info_ = item_info;
}

void UAZAmmoItem::IncreaseCount(int32 count)
{
	info_.item_count = info_.item_count + count;
}

void UAZAmmoItem::DecreaseCount(int32 count)
{
	info_.item_count = info_.item_count - count;
}

int32 UAZAmmoItem::GetItemCount()
{
	return info_.item_count;
}

int32 UAZAmmoItem::GetItemKey()
{
	return info_.item_key;
}

FAmmoInfo UAZAmmoItem::GetItemInfo()
{
	return info_;
}

EStorageType UAZAmmoItem::GetStorageType()
{
	return info_.storage_type;
}
