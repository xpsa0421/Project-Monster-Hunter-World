// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZResourceMgr.h"

UAZResourceMgr::UAZResourceMgr()
{
}

void UAZResourceMgr::Init()
{
}

void UAZResourceMgr::AddResourceReference(const UObject* cache_resource, EAZResourceCacheType cache_type)
{
	switch (cache_type)
	{
	case EAZResourceCacheType::Static: cached_resources_static.Emplace(cache_resource); break;
	case EAZResourceCacheType::MapOnly: cached_resources_map_only.Emplace(cache_resource); break;
	}
}

void UAZResourceMgr::ClearResourceReference(EAZResourceCacheType cache_type)
{
	switch (cache_type)
	{
	case EAZResourceCacheType::Static: cached_resources_static.Empty(); break;
	case EAZResourceCacheType::MapOnly: cached_resources_map_only.Empty(); break;
	}
}
