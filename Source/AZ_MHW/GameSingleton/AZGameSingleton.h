// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "Engine/StreamableManager.h"
#include "AZGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameSingleton : public UObject
{
	GENERATED_BODY()
public:
	UAZGameSingleton();
	
private:
	static UAZGameSingleton* instance_;

public:
	UPROPERTY() class UAZTableMgr* table_mgr;
	UPROPERTY() class UAZResourceMgr* resource_mgr;
	UPROPERTY() class UAZPlayerMgr* player_mgr_;
	UPROPERTY() class UAZMonsterMgr* monster_mgr_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<UAZResourceMgr> resource_mgr_class;

	FStreamableManager streamable_manager;
	
	static UAZGameSingleton* instance();
	void Init();
	void InitResourceMgr();
	class UAZResourceMgr* GetResourceMgr();

public:
	UPROPERTY() float default_scale_factor = -1.0f;
};

#define GetGameSingleton() UAZGameSingleton::instance()

