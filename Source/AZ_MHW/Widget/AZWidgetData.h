#pragma once

#include "AZWidgetData.generated.h"

UENUM(BlueprintType)
enum class ETouchMaskSearchType : uint8
{
	None,

	SlotIndex,

	ItemIndex,
	ItemMakeCategoryIndex,
	ItemMakeGroupIndex,
	CollectionIndex,
	NpcIndex,
	NpcShopItemIndex,
	SkillIndex,

	ClassBranch,
	ClassBranch_ItemIndex,
	ClassBranch_SkillIndex
};

UENUM(BlueprintType)
enum class EAZButtonLockOption : uint8
{
	Dimmed		UMETA(DisplayName = "Lock with Dimmed"),
	Invisible	UMETA(DisplayName = "Lock with Invisible"),
};

UENUM(BlueprintType)
enum class EAZArrowLocateDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EAZTouchMaskShape : uint8
{
	Circle,
	Rect,
};

USTRUCT(BlueprintType)
struct FAZTouchMaskSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) EAZTouchMaskShape shape;
	UPROPERTY(EditAnywhere) FVector2D mask_scale;
	UPROPERTY(EditAnywhere) float arrow_scale;
	UPROPERTY(EditAnywhere) EAZArrowLocateDirection arrow_locate_direction;
	UPROPERTY(EditAnywhere) float arrow_offset;
	UPROPERTY(EditAnywhere) float text_offset;
	UPROPERTY(EditAnywhere) bool is_toggle_type;
};

USTRUCT(BlueprintType)
struct FAZLockOptionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) EAZButtonLockOption button_lock_option;
	UPROPERTY(EditAnywhere) int32 unlock_quest_index;
	uint8 locked : 1;
};