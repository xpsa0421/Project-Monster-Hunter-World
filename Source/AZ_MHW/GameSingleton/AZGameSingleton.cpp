// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
#include "AZ_MHW/Manager/AZResourceMgr.h"
#include "Manager/AZPlayerMgr.h"

UAZGameSingleton* UAZGameSingleton::instance_ = nullptr;

UAZGameSingleton::UAZGameSingleton()
{
	resource_mgr = nullptr;
}

UAZGameSingleton* UAZGameSingleton::instance()
{
	if (instance_ == nullptr)
	{
		instance_ = Cast<UAZGameSingleton>(GEngine->GameSingleton);
		if (instance_ && instance_->IsValidLowLevel())
		{
			instance_->Init();
		}
	}
	return instance_;
}

void UAZGameSingleton::Init()
{
	InitResourceMgr();

	table_mgr = NewObject<UAZTableMgr>();
	table_mgr->LoadAll();
	table_mgr->LoadComplete();

	player_mgr_ = NewObject<UAZPlayerMgr>();
	player_mgr_->Init();

	monster_mgr_ = NewObject<UAZMonsterMgr>(this);
	monster_mgr_->Init();
}

void UAZGameSingleton::InitResourceMgr()
{
	if (resource_mgr == nullptr)
	{
		UClass* object_class = nullptr;//= FSoftClassPath(TEXT("/Game/Blueprint/Resource/BP_ResourceMgr.BP_ResourceMgr_C")).TryLoadClass<UAZResourceMgr>();

		if (object_class)
		{
			resource_mgr = NewObject<UAZResourceMgr>(this, object_class);
		}
		else
		{
			resource_mgr = NewObject<UAZResourceMgr>();
		}

		if (resource_mgr != nullptr)
		{
			resource_mgr->Init();
		}
	}
}

UAZResourceMgr* UAZGameSingleton::GetResourceMgr()
{
	return resource_mgr;
}