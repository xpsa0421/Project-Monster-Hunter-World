// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerMgr.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
#include <AssetRegistry/AssetRegistryModule.h>

//{AssetTypePrefix}_{AssetName}_{Descriptor}_{Version}

/** AssetTypePrefix 에셋유형 접두사
 *M = 머터리얼
 *MI = 머터리얼 인스턴스
 *PM = 피직스 머터리얼
 *PPM = 포스트 프로세스 머터리얼
 *
 *T = 텍스쳐
 *SM = 스태틱 메시
 *SK = 스켈레탈 메시
 *SKEL = 스켈레톤
 *PHYS = 피직스 에셋

 *AM = 애님 몽타주
 *BS = 블렌드스페이스
 *AS = 애님 시퀀스
 
 *BP = 블루프린트
 *ABP = 애니메이션 블루프린트
 *WBP = 위젯 블루프린트
 
 *AC = 액터 컴포넌트

 *DT = 데이터테이블
 *CT = 커브테이블
 
 *E = 열거형
 *F = 구조체
 *BI = 블루프린트 인터페이스
*/

UAZPlayerMgr::UAZPlayerMgr()
{
	//Mesh는 ID 고정경로이기 때문에 테이블로 안해도 될 것같다.
	//(물론 회사에서 리소스도 관리를 자주한다면 테이블에서 찾아서 등록하긴 할 듯하다.
	//if(UAZGameSingleton::instance_->table_mgr->_Load<T>("EquipmentData.csv"))
	//{
	//	UAZGameSingleton::instance_->table_mgr->GetData<class Table*>();
	//}
	
}

void UAZPlayerMgr::Init()
{
	GetSkeletalMesh(10500);
	GetSkeletalMesh(11000);
	GetSkeletalMesh(11500);
	GetSkeletalMesh(12000);
	//GetSkeletalMesh(12500);
}

void UAZPlayerMgr::Shutdown()
{
	anim_montage_map_.Empty();
	anim_sequence_map_.Empty();
	player_character_part_map_.Empty();
	command_bit_mask_map_.Empty();
}


UAnimSequence* UAZPlayerMgr::GetSequence(FName name)
{
	//Find
	if (const auto& anim_sequence_asset = anim_sequence_map_.Find(name))
	{
		return *anim_sequence_asset;
	}
	//Make Path
	TMap<FString, FStringFormatArg> path_string_map;
	path_string_map.Add("AssetPath",TEXT("/Game/AZ/Character/Animation/CO_Sequence"));//CO_Gesture
	path_string_map.Add("AssetType",TEXT("AS"));//AnimComposit? AC
	path_string_map.Add("AssetName",name.ToString());
	//Load
	FString load_path = FString::Format(TEXT("{AssetPath}/{AssetType}_{AssetName}"), path_string_map);
	if (UAnimSequence* load_object = LoadObject<UAnimSequence>(nullptr, *load_path, nullptr, LOAD_None, nullptr))
	{
		anim_sequence_map_.Add(name, load_object);
		return load_object;
	}
	//Failed
	return nullptr;
}
bool UAZPlayerMgr::UnloadSequence(FName name)
{
	if (anim_sequence_map_.Find(name))
	{
		anim_sequence_map_.Remove(name);
		return true;
	}
	return false;
}

UAnimMontage* UAZPlayerMgr::GetMontage(FName name)
{
	//Find
	if (const auto& anim_montage_asset = anim_montage_map_.Find(name))
	{
		return *anim_montage_asset;
	}
	//Make Path
	TMap<FString, FStringFormatArg> path_string_map;
	path_string_map.Add("AssetPath",TEXT("/Game/AZ/Character/Animation/AnimMontage"));
	path_string_map.Add("AssetType",TEXT("AM"));
	path_string_map.Add("CharacterType",TEXT("Player"));
	path_string_map.Add("AssetName",name.ToString());
	//Load
	FString load_path = FString::Format(TEXT("{AssetPath}/{AssetType}_{CharacterType}_{AssetName}"), path_string_map);
	if (UAnimMontage* load_object = LoadObject<UAnimMontage>(nullptr, *load_path, nullptr, LOAD_None, nullptr))
	{
		anim_montage_map_.Add(name, load_object);
		return load_object;
	}
	//Failed
	return nullptr;
}
bool UAZPlayerMgr::UnloadMontage(FName name)
{
	if (anim_montage_map_.Find(name))
	{
		anim_montage_map_.Remove(name);
		return true;
	}
	return false;
}

USkeletalMesh* UAZPlayerMgr::GetSkeletalMesh(int32 item_id)
{
	//isn't Mesh ID
	if (item_id < 1001)
	{
		return nullptr;
	}

	//SKMeshMap
	if (const auto& player_character_part = player_character_part_map_.Find(item_id))
	{
		return *player_character_part;
	}

	//Load
	TMap<FString, FStringFormatArg> path_string_map;
	path_string_map.Add("AssetPath",TEXT("/Game/AZ/Character/Mesh"));
	//PathStringMap.Add("GenderType",TEXT("F"));//TEXT("M")
	path_string_map.Add("AssetType",TEXT("SK"));
	path_string_map.Add("EquipType",TEXT(""));
	int32 item_resource_id = 0;

	//Asset Offset을 Table로 관리한다면 이것도 관리가 가능함.
	if (item_id < 10000)
	{
		if (item_id < 1501)//대검
		{
			path_string_map["EquipType"] = FStringFormatArg("WP00");
			item_resource_id = item_id - 1000;
		}
		else if (item_id < 2001)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP01");
			item_resource_id = item_id - 1500;
		}
		else if (item_id < 2501)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP02");
			item_resource_id = item_id - 2000;
		}
		else if (item_id < 3001)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP03");
			item_resource_id = item_id - 2500;
		}
		else if (item_id < 3501)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP04");
			item_resource_id = item_id - 3000;
		}
		else if (item_id < 4001)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP05");
			item_resource_id = item_id - 3500;
		}
		else if (item_id < 4501)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP06");
			item_resource_id = item_id - 4000;
		}
		else if (item_id < 5001)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP07");
			item_resource_id = item_id - 4500;
		}
		else if (item_id < 5501)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP08");
			item_resource_id = item_id - 5000;
		}
		else if (item_id < 6001)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP09");
			item_resource_id = item_id - 5500;
		}
		else if (item_id < 6501)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP10");
			item_resource_id = item_id - 6000;
		}
		else if (item_id < 7001)//활
		{
			path_string_map["EquipType"] = FStringFormatArg("WP11");
			item_resource_id = item_id - 6500;
		}
		else if (item_id < 7501)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP12");
			item_resource_id = item_id - 7000;
		}
		else if (item_id < 8001)
		{
			path_string_map["EquipType"] = FStringFormatArg("WP13");
			item_resource_id = item_id - 7500;
		}
	}
	else if (item_id < 12600)
	{
		if (item_id < 10501)
		{
			path_string_map["EquipType"] = FStringFormatArg("FBody");
			item_resource_id = item_id - 10000;
		}
		else if (item_id < 11001)
		{
			path_string_map["EquipType"] = FStringFormatArg("FLeg");
			item_resource_id = item_id - 10500;
		}
		else if (item_id < 11501)
		{
			path_string_map["EquipType"] = FStringFormatArg("FArm");
			item_resource_id = item_id - 11000;
		}
		else if (item_id < 12001)
		{
			path_string_map["EquipType"] = FStringFormatArg("FWaist");
			item_resource_id = item_id - 11500;
		}
		else if (item_id < 12501)
		{
			path_string_map["EquipType"] = FStringFormatArg("FHead");
			item_resource_id = item_id - 12000;
		}
		else if (item_id >= 12501)
		{
			path_string_map["EquipType"] = FStringFormatArg("FHair");
			item_resource_id = item_id - 12500;
		}
		else
		{
			return nullptr;
		}
	}
	
	FString load_path = FString::Format(TEXT("{AssetPath}/{EquipType}/{AssetType}_{EquipType}_"), path_string_map);
	load_path.Appendf(TEXT("%03d"), item_resource_id);//FString::Printf(TEXT("%03d"),ItemPath)//Encoding Warning

	if (USkeletalMesh* load_object = LoadObject<USkeletalMesh>(nullptr, *load_path, nullptr, LOAD_None, nullptr))
	{
		player_character_part_map_.Add(item_id, load_object);
		return load_object;
	}

	return nullptr;
}
bool UAZPlayerMgr::UnloadSkeletalMesh(int32 item_id)
{
	if (player_character_part_map_.Find(item_id))
	{
		//player_character_part_map_[item_id] = nullptr;?
		player_character_part_map_.Remove(item_id);
		return true;
	}
	return false;
}



void UAZPlayerMgr::SetCommandBitMaskMap(int32 weapon_type)
{
	command_bit_mask_map_.Empty();

	FString input_command_data_path = TEXT("/Game/AZ/DataAsset/Command/DT_InputCommandData.DT_InputCommandData");
	static ConstructorHelpers::FObjectFinder<UDataTable> input_command_table(*input_command_data_path);
	check(input_command_table.Succeeded());
	command_bitmask_table_ = input_command_table.Object;
	check(command_bitmask_table_->GetRowMap().Num() > 0);
	
	//MyCustomTableData* GetData(T key)
	//{
	// return command_bitmask_talbe->FindRow<FInputCommandData>(*FString::FromInt(key),TEXT(""))
	//}
	//Load
	//for(LoadCount)
	{
		//CommandBitMaskMap.Add(,);
	}
}

uint32 UAZPlayerMgr::GetCommandBitMask(FName name)
{
	if (const auto& BitMask = command_bit_mask_map_.Find(name))
	{
		return *BitMask;
	}
	return 0;
}
