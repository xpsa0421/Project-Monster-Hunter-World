// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Math/NumericLimits.h>
#include "AZEnum.generated.h"

UENUM(BlueprintType)
enum class EAutoMove : uint8
{
	None
};

UENUM(BlueprintType)
enum class ESocketHolderType : uint8
{
	Game,
	Max
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	None,
	GameDefault
};

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	None,
	Login,
	CharacterSelect,
	CharacterCreate,
	Lobby,
	InGame,
};

UENUM(BlueprintType)
enum class EGameModeFlag : uint8
{
	None,
	Launcher,
	Login,
	Lobby,
	CharacterCreate,
	CharacterSelect,
	InGame
};


// -------------------------------------------------
// Character Common

UENUM(BlueprintType)
enum class EAttackEffectType : uint8
{
	None,
	KnockBack,
	RoarStagger,
	Stun,
	Trip,
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	// Hunter
	Cut,
	Blunt,
	Ammo,

	// Monster
	Base,
	Fire,
	Poison,

	None,
};

// End of Character
// -------------------------------------------------
// Monster

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	None,
	NPC,
	Player,
	Monster,	
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Locomotion,
	Action,
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	None,
	StopMove,
	Walk,
	Run,
	Fly,
};

UENUM(BlueprintType)
enum class EBossRank : uint8
{
	None,
	Low,
	Medium,
	Master,
};

UENUM(BlueprintType, Meta = (Experimental))
enum class EMonsterGoalSearchStyle : uint8
{
	Random,
	Chase,
	Escape,
};

UENUM(BlueprintType)
enum class EMonsterBehaviorType : uint8
{
	Neutral,
	Friendly,
	Hostile,
};

UENUM(BlueprintType)
enum class EMonsterActionMode : uint8
{
	Normal,
	Transition,
	Combat,
};

UENUM(BlueprintType)
enum class EMonsterActionPriority : uint8
{
	None,
	Locomotion,
	Action,
	MoveModeChange,
	Hit,
	Debuff,
	Death,
	Cinematic,
};

UENUM(BlueprintType)
enum class EEffectDurationType : uint8
{
	Instant,
	ForDuration,
};

//TODO for now, no triggers are set for combat actions
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMonsterActionTriggerType : uint8
{
	None			= 0,		
	Transition		= 1 << 0,	// used for transition actions only
	Auto			= 1 << 1,
	Damage			= 1 << 2,	// used for transition actions only
	Sight			= 1 << 3,	// used for transition actions only
	InRange			= 1 << 4,	// used for noncombat actions only
};
ENUM_CLASS_FLAGS(EMonsterActionTriggerType);

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMonsterActionConditionType : uint8
{
	None		= 0,		
	Flying		= 1 << 0,	 // if false, must be on ground
	InRange		= 1 << 1,	
	Health		= 1 << 2,	
};
ENUM_CLASS_FLAGS(EMonsterActionConditionType);

UENUM(BlueprintType, Meta = (Experimental))
enum class ESearchEnemyType : uint8
{
	None,
	Distance,
	Damage
};

namespace AZBlackboardKey
{
	FName const ai_state("AIState");
	FName const move_state("MoveState");
	FName const action_mode("ActionMode");
	FName const target_character("TargetCharacter");
	FName const target_location("TargetLocation");
	FName const patrol_delay("PatrolDelay");
	FName const action_delay("ActionDelay");
	FName const is_triggered_by_sight("IsTriggeredBySight");
}

UENUM(BlueprintType)
enum class EMonsterBodyPart : uint8
{
	Default,
	Head,
	RightWing,
	LeftWing,
	Tail,
	Neck,
	Body,
	Back,
	Leg,
	Eyelid,	
	Eyeball,	// not used
	Max,
};

UENUM()
enum class EMonsterBodyPartChangeType : uint8
{
	Wound,
	Break,
	Sever,
};

UENUM()
enum class EMoveRequestResult : uint8
{
	Failed,
	RequestSuccessful,
	AlreadyAtGoal,
};

UENUM()
enum class EPostProcessType : uint8
{
	None,
	RadialBlur,
};

// End of Monster
// -------------------------------------------------