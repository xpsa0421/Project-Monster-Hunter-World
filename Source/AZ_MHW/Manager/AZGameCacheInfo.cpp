// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZGameCacheInfo.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "Internationalization/TextKey.h"

int32 UAZGameCacheInfo::alloc_character_index_ = 0;

UAZGameCacheInfo::UAZGameCacheInfo()
{
}

void UAZGameCacheInfo::Init()
{
	game_instance_ = Cast<UAZGameInstance>(GetOuter());
}

uint32 UAZGameCacheInfo::GetIDHashCode(FString& id)
{
	auto hash_code = TextKeyUtil::HashString(*id);
	return hash_code;
}

bool UAZGameCacheInfo::GetID(int32 id_hash, FString& out_id)
{
	auto find_id = id_check_.Find(id_hash);
	if (find_id == nullptr)
	{
		return false;
	}

	out_id = *find_id;
	return true;
}

TArray<FCharacterSimpleInfo> UAZGameCacheInfo::GetCharacterSimpleInfoArray(uint32 client_index)
{
	uint32 id_hash = GetIdHashCodeFromClientIndex(client_index);
	auto character_infos = character_infos_.FindOrAdd(id_hash);
	return character_infos;
}

uint32 UAZGameCacheInfo::AllocCharacterIndex()
{
	return ++alloc_character_index_;
}

uint32 UAZGameCacheInfo::CreateCharacterSimpleInfo(uint32 id_hash, const FCharacterSimpleInfo& simple_info)
{
	FCharacterSimpleInfo set_simple_info;
	set_simple_info = simple_info;
	set_simple_info.character_index_ = AllocCharacterIndex();
	character_infos_.FindOrAdd(id_hash).Add(set_simple_info);
	character_info_.Add(set_simple_info.character_index_, set_simple_info);
	return set_simple_info.character_index_;
}

UAZInventoryManager* UAZGameCacheInfo::GetInventoryManager(int32 character_index)
{
	auto character_info = character_inventory_.Find(character_index);
	if (character_info == nullptr)
	{
		return nullptr;
	}
	return *character_info;
}

FCharacterSimpleInfo UAZGameCacheInfo::GetCharacterSimpleInfo(int32 character_index)
{
	auto simple_info = character_info_.Find(character_index);
	if (simple_info == nullptr)
	{
		AZ_PRINT_LOG_IF_FALSE(simple_info, "[GetCharacterSimpleInfo simple_info is nullptr]");
		return FCharacterSimpleInfo();
	}

	return *simple_info;
}

void UAZGameCacheInfo::SetClientId(FString& id)
{
	id_ = id;
	id_hash_ = GetIDHashCode(id_);
}

void UAZGameCacheInfo::AddCharacterSimpleInfo(FCharacterSimpleInfo& character_info)
{
	character_infos_.FindOrAdd(id_hash_).Add(character_info);
	character_info_.FindOrAdd(character_info.character_index_) = character_info;
	// 일방통행이므로 아이템 생성해줌
	character_inventory_.Add(character_info.character_index_, NewObject<UAZInventoryManager>(this));
	character_inventory_[character_info.character_index_]->Init();
}

TArray<FCharacterSimpleInfo> UAZGameCacheInfo::GetCurrentCharacterSimpleInfoArray()
{
	return GetCharacterSimpleInfoArray(client_index_);
}

UAZInventoryManager* UAZGameCacheInfo::GetCurrentCharacterItem()
{
	return character_inventory_[current_character_index_];
}

uint32 UAZGameCacheInfo::GetIdHashCodeFromClientIndex(uint32 client_index)
{
	auto id_hash_code = client_index_to_id_hash_code_.Find(client_index);
	if (id_hash_code == nullptr)
	{
		return 0;
	}
	return *id_hash_code;
}

bool UAZGameCacheInfo::SignupRequest(FString& id)
{
	auto hash_code = GetIDHashCode(id);
	auto temp_string = id_check_.Find(hash_code);
	// 1. 이미 아이디가 존재
	if (temp_string != nullptr)
	{
		return false;
	}

	id_check_.Add(hash_code, id);
	character_infos_.FindOrAdd(hash_code);
	return true;
}

bool UAZGameCacheInfo::LoginRequest(uint32 client_index, FString& id)
{
	auto hash_code = GetIDHashCode(id);
	client_index_to_id_hash_code_.Add(client_index, hash_code);
	return true;
}

bool UAZGameCacheInfo::LoginResponse(uint32 client_index)
{
	auto hash_code = GetIDHashCode(id_);
	client_index_ = client_index;
	client_index_to_id_hash_code_.Add(client_index, hash_code);
	return true;
}

void UAZGameCacheInfo::ResetCharacterInfo()
{
	character_infos_.Empty();
	character_info_.Empty();
}

bool UAZGameCacheInfo::RemoveClientIndexToIdHashCode(uint32 client_index)
{
	client_index_to_id_hash_code_.Remove(client_index);
	return true;
}

void UAZGameCacheInfo::PlayableCharacterDataRequest(UINT32 client_index)
{
	auto* hash_code = client_index_to_id_hash_code_.Find(client_index);
	if (hash_code == nullptr)
	{
		AZ_PRINT_LOG_IF_FALSE(hash_code, "[PlayableCharacterDataRequest hash_code_nullptr]");
		return;
	}

	auto character_simple_infos = character_infos_.FindOrAdd(*hash_code);
	SC_PLAYER_PLAYABLE_CHARACTER_DATA_RES packet;
	packet.count = 0;
	for (auto& character_info : character_simple_infos)
	{
		if (packet.count == 0)
		{
			packet.info_0 = character_info;
		}
		if (packet.count == 1)
		{
			packet.info_1 = character_info;
		}
		if (packet.count == 2)
		{
			packet.info_2 = character_info;
		}
		packet.count++;
	}
	game_instance_->SendPacketFunc(client_index, sizeof(packet), (char*)&packet);
}

void UAZGameCacheInfo::PlayerCharacterCreateRequest(UINT32 client_index, FCharacterSimpleInfo& simple_info)
{
	auto* hash_code = client_index_to_id_hash_code_.Find(client_index);
	if (hash_code == nullptr)
	{
		AZ_PRINT_LOG_IF_FALSE(hash_code, "[PlayerCharacterCreateRequest hash_code_nullptr]");
		return;
	}

	// 오류검사 안함 일단 생성해줌
	int32 charcter_index = CreateCharacterSimpleInfo(*hash_code, simple_info);
	auto create_simple_info = GetCharacterSimpleInfo(charcter_index);
	// 그냥 기본아이템 로직은 동일하니 생성후 패킷은 안보냄(클라에서도 동일하게 만들 것임)
	character_inventory_.Add(charcter_index, NewObject<UAZInventoryManager>(this));
	character_inventory_[charcter_index]->Init();

	SC_PLAYER_CHARACTER_CREATE_RES packet;
	packet.create_info = create_simple_info;
	game_instance_->SendPacketFunc(client_index, sizeof(packet), (char*)&packet);
}
