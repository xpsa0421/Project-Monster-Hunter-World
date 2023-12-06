#pragma once

#include "AZ_MHW/CommonSource/AZEnum.h"
#include "MapStruct.generated.h"

UENUM(BlueprintType)
enum class EMapType : uint8
{
	None,
	Login,
	Lobby,
	Launcher,
	CharacterCreate,
	CharacterSelect,
	Dungeon,
	Max
};

UENUM(BlueprintType)
enum class EWarpType : uint8
{
	None,
	Max
};

USTRUCT(BlueprintType)
struct FSpawnObjectInfo
{
	GENERATED_BODY()

	EObjectType object_type;
	int32 table_index;
	int32 map_index;

public:
	FSpawnObjectInfo() : object_type(EObjectType::Player)
		, table_index(0)
		, map_index(0)
	{}

};