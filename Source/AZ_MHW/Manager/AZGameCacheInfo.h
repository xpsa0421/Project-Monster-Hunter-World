// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/Manager/AZInventoryManager.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZGameCacheInfo.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZGameCacheInfo : public UObject
{
	GENERATED_BODY()

public:
	UAZGameCacheInfo();
	void Init();

private:
	// 공용
	TMap<uint32/*client_index*/, uint32/*id_hash*/> client_index_to_id_hash_code_;
	static int32 alloc_character_index_;
	TMap<uint32/*id_hash*/, FString> id_check_;//해시코드 일치 시 중복아이디로 체크하기

	TMap<uint32/*id_hash*/, TArray<FCharacterSimpleInfo>> character_infos_; // 캐릭터 정보
	TMap<int32/*character_index*/, FCharacterSimpleInfo> character_info_;

	UPROPERTY() TMap<int32/*character_index*/, class UAZInventoryManager*> character_inventory_;

public:
	// 공용 함수
	uint32 GetIDHashCode(FString& id);
	bool GetID(int32 id_hash, FString& out_id);
	TArray<FCharacterSimpleInfo> GetCharacterSimpleInfoArray(uint32 client_index);
	UAZInventoryManager* GetInventoryManager(int32 character_index);
	FCharacterSimpleInfo GetCharacterSimpleInfo(int32 character_index);

	// 클라 전용
public:
	FString id_;
	uint32 id_hash_;
	uint32 client_index_;
	int32 current_character_index_;
	FString current_character_nickname_;
	void SetClientId(FString& id);
	void AddCharacterSimpleInfo(FCharacterSimpleInfo& character_info);
	TArray<FCharacterSimpleInfo> GetCurrentCharacterSimpleInfoArray();
	UAZInventoryManager* GetCurrentCharacterItem();
	bool LoginResponse(uint32 client_index);
	void ResetCharacterInfo();

	// 서버
public:
	uint32 GetIdHashCodeFromClientIndex(uint32 client_index);

	bool SignupRequest(FString& id);
	bool LoginRequest(uint32 client_index, FString& id);
	
	bool RemoveClientIndexToIdHashCode(uint32 client_index);

	void PlayableCharacterDataRequest(UINT32 client_index);
	void PlayerCharacterCreateRequest(UINT32 client_index, FCharacterSimpleInfo& simple_info_);

	uint32 AllocCharacterIndex();
	uint32 CreateCharacterSimpleInfo(uint32 id_hash, const FCharacterSimpleInfo& simple_info);

private:
	UPROPERTY() class UAZGameInstance* game_instance_;
};
