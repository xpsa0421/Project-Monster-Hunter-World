// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "Engine/DataTable.h"
#include "AZPlayerMgr.generated.h"


USTRUCT(BlueprintType)
struct FInputCommandData : public FTableRowBase
{
	GENERATED_BODY()

	FInputCommandData()
	{
		montage_name  = NAME_None;
		section_name = NAME_None;
		state_mask  = 0;

		input_mask = 0;

		next_montage_name = NAME_None;
		next_section_name = NAME_None;
		next_state_mask = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName montage_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName section_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 state_mask;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 input_mask;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName next_montage_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName next_section_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 next_state_mask;
};

USTRUCT(BlueprintType)
struct FAnimationData
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName next_montage_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName next_section_name;
};

/**
 * GameInstance에 넣으면 생성시점이 Play이후이므로 실행중이지 않은 Editor에서 호출시 터진다.
 * GameSingleton을 사용해야 한다?
 */
UCLASS()
class AZ_MHW_API UAZPlayerMgr : public UObject
{
	GENERATED_BODY()
public:
	UAZPlayerMgr();

	void Init();
	void Shutdown();
	
#pragma region Inherited function
protected:
	
#pragma endregion
public:
	UPROPERTY() TMap<FName, UAnimSequence*> anim_sequence_map_;
	UPROPERTY() TMap<FName, UAnimMontage*> anim_montage_map_;
	UPROPERTY() TMap<int32, USkeletalMesh*> player_character_part_map_;//끝
	UPROPERTY() TMap<FName, uint32> command_bit_mask_map_;
	UPROPERTY() UDataTable* command_bitmask_table_;
	
	UFUNCTION() UAnimSequence* GetSequence(FName name);
	UFUNCTION() bool UnloadSequence(FName name);
	
	UFUNCTION() UAnimMontage* GetMontage(FName name);
	UFUNCTION() bool UnloadMontage(FName name);

	UFUNCTION() USkeletalMesh* GetSkeletalMesh(int32 item_id);
	UFUNCTION() bool UnloadSkeletalMesh(int32 item_id);

	
	UFUNCTION() void SetCommandBitMaskMap(int32 weapon_type);
	UFUNCTION() uint32 GetCommandBitMask(FName name);

	//UPROPERTY() TMap<FName, TMap<int32,FName>> MaskMap;
	
	
};
