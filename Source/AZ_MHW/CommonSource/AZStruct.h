#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "VectorTypes.h"
#include "AZStruct.generated.h"

//============================================
// Character Common

USTRUCT(BlueprintType)
struct FCharacterSimpleInfo
{
	GENERATED_BODY()
	
	int32 character_index_;
	char character_nick_[33];
	UPROPERTY() FVector4f hair_color_;
	UPROPERTY() int32 hair_id_;
	UPROPERTY() int32 head_id_;
	UPROPERTY() int32 body_id_;
	UPROPERTY() int32 arm_id_;
	UPROPERTY() int32 leg_id_;
	UPROPERTY() int32 waist_id_;
	UPROPERTY() int32 weapon_type;
	UPROPERTY() int32 weapon_id;
	
	FCharacterSimpleInfo()
	{
		character_index_;
		memset(character_nick_, 0, sizeof(character_nick_));
		hair_color_ = FVector4f(1,1,1,1);
		hair_id_= 12601;
		head_id_= 12500;
		body_id_= 10500;
		arm_id_= 11500;
		leg_id_= 11000;
		waist_id_ = 12000;

		weapon_type = 0;
		weapon_id = 1001;
	}

	FCharacterSimpleInfo& operator=(const FCharacterSimpleInfo& ref)
	{
		character_index_ = ref.character_index_;
		memcpy(character_nick_, ref.character_nick_, sizeof(character_nick_));
		hair_color_ = ref.hair_color_;
		hair_id_ = ref.hair_id_;
		head_id_= ref.head_id_;
		body_id_= ref.body_id_;
		arm_id_= ref.arm_id_;
		leg_id_= ref.leg_id_;
		waist_id_ = ref.waist_id_;

		weapon_type = ref.weapon_type;
		weapon_id = ref.weapon_id;
		return *this;
	}
};

// for blueprint usages
USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) EDamageType type;
	UPROPERTY(EditAnywhere) int32 amount;

	FDamageInfo() : type(EDamageType::None), amount(0) {};
	FDamageInfo(EDamageType type, int32 amount) : type(type), amount(amount) {};
};

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TArray<FDamageInfo> damage_array;
	UPROPERTY(EditAnywhere) EAttackEffectType attack_effect;

	FAttackInfo() : attack_effect(EAttackEffectType::None) {};
	int32 GetDamageTotal()
	{
		int32 total_damage = 0;
		for (const auto damage_info : damage_array)
		{
			total_damage += damage_info.amount;
		}
		return total_damage;
	}
	FString GetDebugString()
	{
		FString debug_str;
		for (auto damage_info : damage_array)
		{
			debug_str += FString::Printf(TEXT("[%s] %d "), *UAZUtility::EnumToString(damage_info.type), damage_info.amount);
		}
		return debug_str;
	}
};

// End of Character Common
//============================================
// Monster

USTRUCT(BlueprintType)
struct FHitResultInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 damage_amount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FVector trace_start;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FVector trace_end;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FVector hit_location;

	FHitResultInfo() : damage_amount(0), trace_start(FVector::ZeroVector), trace_end(FVector::ZeroVector), hit_location(FVector::ZeroVector) {};
	FHitResultInfo(int32 damage, FVector start, FVector end, FVector hit) :
		damage_amount(damage), trace_start(start), trace_end(end), hit_location(hit) {};
};

USTRUCT(BlueprintType)
struct FMonsterActionStateInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EMonsterActionMode action_mode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EMonsterActionPriority priority_score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EMoveState move_state;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FName animation_name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FName montage_section_name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float target_angle;

	FMonsterActionStateInfo()
	{
		action_mode = EMonsterActionMode::Normal;
		priority_score = EMonsterActionPriority::None;
		move_state = EMoveState::None;
		animation_name = montage_section_name = NAME_None;
		target_angle = 0.0f;
	}
	void Reset()
	{
		action_mode = EMonsterActionMode::Normal;
		priority_score = EMonsterActionPriority::Locomotion;
		move_state = EMoveState::None;
		animation_name = NAME_None;
		montage_section_name = NAME_None;
		target_angle = 0.0f;
	}
};

// for server-client transmission
struct MonsterActionStateInfo
{
	EMonsterActionMode action_mode;
	EMonsterActionPriority priority_score;
	EMoveState move_state;
	char animation_name[33];
	char montage_section_name[33];
	float target_angle;

	MonsterActionStateInfo()
	{
		action_mode = EMonsterActionMode::Normal;
		priority_score = EMonsterActionPriority::None;
		move_state = EMoveState::None;
		animation_name[0] = montage_section_name[0] = '\0';
		target_angle = 0.0f;
	}
	MonsterActionStateInfo(FMonsterActionStateInfo state_info)
	{
		action_mode = state_info.action_mode;
		priority_score = state_info.priority_score;
		move_state = state_info.move_state;
		target_angle = state_info.target_angle;
		strcpy_s(animation_name, sizeof(animation_name), UAZUtility::FNameToCharArr(state_info.animation_name));
		strcpy_s(montage_section_name, sizeof(montage_section_name), UAZUtility::FNameToCharArr(state_info.montage_section_name));
	}
	FMonsterActionStateInfo Convert()
	{
		FMonsterActionStateInfo state_info;
		state_info.action_mode = this->action_mode;
		state_info.priority_score = this->priority_score;
		state_info.move_state = this->move_state;
		state_info.animation_name = UAZUtility::CharArrToFName(this->animation_name);
		state_info.montage_section_name = UAZUtility::CharArrToFName(this->montage_section_name);
		state_info.target_angle = this->target_angle;

		return state_info;
	}
};

USTRUCT(BlueprintType)
struct FBossRageStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 required_damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float duration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float outgoing_damage_multiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float incoming_damage_multiplier;

	FBossRageStats()
	{
		required_damage = duration = outgoing_damage_multiplier = incoming_damage_multiplier = 0.0f;
	};
	FBossRageStats(const int32 required_damage, const int32 duration, const int32 outgoing_damage_multiplier, const int32 incoming_damage_multiplier)
	{
		this->required_damage = required_damage;
		this->duration = UAZUtility::MillisecondsToSeconds(duration);
		this->outgoing_damage_multiplier = UAZUtility::PerTenThousandToPerOne(outgoing_damage_multiplier);
		this->incoming_damage_multiplier = UAZUtility::PerTenThousandToPerOne(incoming_damage_multiplier);
	}
	FBossRageStats(const int32 required_damage, const float duration, const float outgoing_damage_multiplier, const float incoming_damage_multiplier)
	{
		this->required_damage = required_damage;
		this->duration = duration;
		this->outgoing_damage_multiplier = outgoing_damage_multiplier;
		this->incoming_damage_multiplier = incoming_damage_multiplier;
	}
};

USTRUCT(BlueprintType)
struct FBossEscapeStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 num_allowed_escapes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<float> escape_health_ratios;

	FBossEscapeStats()
	{
		num_allowed_escapes = 0;
	};
	FBossEscapeStats(const int32 num_allowed_escapes, const TArray<float>& escape_health_ratios)
	{
		this->num_allowed_escapes = num_allowed_escapes;
		this->escape_health_ratios = escape_health_ratios;
	}
	FBossEscapeStats(const int32 num_allowed_escapes, const TArray<int32>& escape_health_ratios)
	{
		this->num_allowed_escapes = num_allowed_escapes;
		for (const int32& escape_health_ratio : escape_health_ratios)
		{
			this->escape_health_ratios.Add(UAZUtility::PerTenThousandToPerOne(escape_health_ratio));
		}
	}
};

USTRUCT(BlueprintType)
struct FBossBodyPartState
{
	GENERATED_BODY()

	// Wound 	
	UPROPERTY(VisibleAnywhere) bool is_woundable;
	UPROPERTY(VisibleAnywhere) bool is_wounded;
	UPROPERTY(VisibleAnywhere) int32 wound_required_damage;
	UPROPERTY(VisibleAnywhere) int32 wound_accumulated_damage;
	
	// Part break 
	UPROPERTY(VisibleAnywhere) bool is_breakable;
	UPROPERTY(VisibleAnywhere) bool is_broken;
	UPROPERTY(VisibleAnywhere) int32 break_required_damage;
	UPROPERTY(VisibleAnywhere) int32 break_accumulated_damage;

	// Sever 
	UPROPERTY(VisibleAnywhere) bool is_severable;
	UPROPERTY(VisibleAnywhere) bool is_severed;
	UPROPERTY(VisibleAnywhere) int32 sever_required_damage;
	UPROPERTY(VisibleAnywhere) int32 sever_accumulated_damage;
	
	UPROPERTY(VisibleAnywhere) bool is_stunnable;

	FBossBodyPartState()
	{
		is_breakable = is_severable = is_woundable = is_stunnable = false;
		is_wounded = is_broken = is_severed = false;
		break_required_damage = break_accumulated_damage = 0;
		sever_required_damage = sever_accumulated_damage = 0;
		wound_required_damage = wound_accumulated_damage = 0;
	}
	FBossBodyPartState(TArray<int32> damage_info) : FBossBodyPartState()
	{
		// Wound
		if (damage_info[0] != -1)
		{
			is_woundable = true;
			wound_required_damage = damage_info[2];
		}
		// Break
		if (damage_info[1] != -1)
		{
			is_breakable = true;
			break_required_damage = damage_info[0];
		}
		// Sever
		if (damage_info[2] != -1)
		{
			is_severable = true;
			sever_required_damage = damage_info[1];
		}
	}
};

// Used for packet only
USTRUCT(BlueprintType)
struct FBossBodyPartDebuffState
{
	GENERATED_BODY()
	
	bool is_wounded;
	bool is_broken;
	bool is_severed;

	FBossBodyPartDebuffState()
	{
		is_wounded = is_broken = is_severed = false;
	}
	FBossBodyPartDebuffState(FBossBodyPartState* state)
	{
		this->is_wounded = state->is_wounded;
		this->is_broken = state->is_broken;
		this->is_severed = state->is_severed;
	}
};

USTRUCT(BlueprintType)
struct FMonsterSightConfigs
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 radius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 lose_radius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float fov;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float max_age;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 auto_success_range;

	FMonsterSightConfigs()
	{
		radius = 1000;
		lose_radius = 1200;
		fov = 70.0f;
		max_age = 3.0f;
		auto_success_range = 300.0f;
	};
	FMonsterSightConfigs(int32 radius, int32 lose_radius, int32 fov, float max_age, int32 auto_success_range)
	{
		this->radius = radius;
		this->lose_radius = lose_radius;
		this->fov = fov;
		this->max_age = max_age;
		this->auto_success_range = auto_success_range;
	}
};

USTRUCT(BlueprintType)
struct FBossWeaknessStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere) TArray<int32> head;
	UPROPERTY(VisibleAnywhere) TArray<int32> neck;
	UPROPERTY(VisibleAnywhere) TArray<int32> body;
	UPROPERTY(VisibleAnywhere) TArray<int32> tail;
	UPROPERTY(VisibleAnywhere) TArray<int32> wing;
	UPROPERTY(VisibleAnywhere) TArray<int32> leg;
	TArray<TArray<int32>> weakness_array;
	TMap<EMonsterBodyPart, TArray<int32>*> weakness_map;
	
	FBossWeaknessStats() {};
	FBossWeaknessStats(const TArray<int32>& head_weakness, const TArray<int32>& neck_weakness,
		const TArray<int32>& body_weakness,	const TArray<int32>& tail_weakness,
		const TArray<int32>& wing_weakness, const TArray<int32>& leg_weakness)
	{
		// cut - blunt - ammo
		head = head_weakness;
		neck = neck_weakness;
		body = body_weakness;
		tail = tail_weakness;
		wing = wing_weakness;
		leg  = leg_weakness;

		TArray<int32> empty_array;
		weakness_array = { empty_array, head_weakness, wing_weakness, wing_weakness,
			tail_weakness, neck_weakness, body_weakness, body_weakness, leg_weakness };

		weakness_map.Add(EMonsterBodyPart::Head, &head);
		weakness_map.Add(EMonsterBodyPart::RightWing, &wing);
		weakness_map.Add(EMonsterBodyPart::LeftWing, &wing);
		weakness_map.Add(EMonsterBodyPart::Tail, &tail);
		weakness_map.Add(EMonsterBodyPart::Neck, &neck);
		weakness_map.Add(EMonsterBodyPart::Body, &body);
		weakness_map.Add(EMonsterBodyPart::Back, &body);
		weakness_map.Add(EMonsterBodyPart::Leg, &leg);
	}
};

USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere) int32 monster_id;
	UPROPERTY(VisibleAnywhere) EMonsterBehaviorType behavior_type;
	UPROPERTY(VisibleAnywhere) int32 base_hp;
	UPROPERTY(VisibleAnywhere) FMonsterSightConfigs sight_configs;
	UPROPERTY(VisibleAnywhere) int32 patrol_range;
	UPROPERTY(VisibleAnywhere) float patrol_delay;
	UPROPERTY(VisibleAnywhere) int32 percept_radius;
	UPROPERTY(VisibleAnywhere) FName name;

	FMonsterInfo()
	{
		monster_id = -1;
		name = NAME_None;
		behavior_type = EMonsterBehaviorType::Neutral;
		base_hp = 1000;
		sight_configs = FMonsterSightConfigs();
		patrol_range = 1000;
		patrol_delay = 5.0f;
		percept_radius = 1500;
	}
};

USTRUCT(BlueprintType)
struct FBossInfo
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 boss_id;
	UPROPERTY(VisibleAnywhere) int32 monster_id;
	UPROPERTY(VisibleAnywhere) EBossRank rank;
	UPROPERTY(VisibleAnywhere) int32 base_stamina;
	UPROPERTY(VisibleAnywhere) float tired_duration;
	UPROPERTY(VisibleAnywhere) bool has_transition_animation;
	UPROPERTY(VisibleAnywhere) FBossEscapeStats escape_stats;
	UPROPERTY(VisibleAnywhere) FBossWeaknessStats weakness_stats;
	UPROPERTY(VisibleAnywhere) FBossBodyPartState head_state;
	UPROPERTY(VisibleAnywhere) FBossBodyPartState body_state;
	UPROPERTY(VisibleAnywhere) FBossBodyPartState wing_state;
	UPROPERTY(VisibleAnywhere) FBossBodyPartState tail_state;
	UPROPERTY(VisibleAnywhere) FBossBodyPartState leg_state;
	UPROPERTY(VisibleAnywhere) TArray<EMonsterBodyPart> stunnable_parts;
	UPROPERTY(VisibleAnywhere) FBossRageStats rage_stats;
	UPROPERTY(VisibleAnywhere) float tenderised_damage_multiplier;

	FBossInfo()
	{
		boss_id = -1;
		monster_id = -1;
		rank = EBossRank::None;
		base_stamina = 0;
		tired_duration = 0;
		has_transition_animation = false;
		tenderised_damage_multiplier = 1.0f;
	}
};

USTRUCT(BlueprintType)
struct FMonsterCombatActionInfo
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 action_id;
	UPROPERTY(VisibleAnywhere) int32 monster_id;

	// Animation info
	UPROPERTY(VisibleAnywhere) FName animation_name;
	UPROPERTY(VisibleAnywhere) FName montage_section_name;

	// Triggers and conditions
	UPROPERTY(VisibleAnywhere) EMonsterActionTriggerType triggers;
	UPROPERTY(VisibleAnywhere) EMonsterActionConditionType conditions;
	UPROPERTY(VisibleAnywhere) int32 condition_min_distance_from_target;
	UPROPERTY(VisibleAnywhere) int32 condition_max_distance_from_target;
	
	UPROPERTY(VisibleAnywhere) float attack_delay;

	FMonsterCombatActionInfo() : action_id(-1), monster_id(-1), animation_name(NAME_None),
		montage_section_name(NAME_None), triggers(EMonsterActionTriggerType::None), conditions(EMonsterActionConditionType::None),
		condition_min_distance_from_target(-1), condition_max_distance_from_target(-1), attack_delay(0) {}
};

USTRUCT(BlueprintType)
struct FMonsterNonCombatActionInfo
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 action_id;
	UPROPERTY(VisibleAnywhere) int32 monster_id;
	UPROPERTY(VisibleAnywhere) FName animation_name;
	UPROPERTY(VisibleAnywhere) FName montage_section_name;
	UPROPERTY(VisibleAnywhere) EMonsterActionConditionType conditions;
	UPROPERTY(VisibleAnywhere) float condition_min_health_ratio;		// only checked if condition includes Health
	UPROPERTY(VisibleAnywhere) float condition_max_health_ratio;

	FMonsterNonCombatActionInfo() : action_id(-1), monster_id(-1), animation_name(NAME_None),
		montage_section_name(NAME_None), conditions(EMonsterActionConditionType::None),
		condition_min_health_ratio(0), condition_max_health_ratio(0){}
};

USTRUCT(BlueprintType)
struct FMonsterSpawnInfo
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 monster_id;
	UPROPERTY(VisibleAnywhere) EBossRank rank;
	UPROPERTY(VisibleAnywhere) FVector location;
	UPROPERTY(VisibleAnywhere) FRotator rotation;

	FMonsterSpawnInfo()
		: monster_id(-1), rank(EBossRank::Low), location(FVector::ZeroVector), rotation(FRotator::ZeroRotator) {}
	FMonsterSpawnInfo(int32 id, EBossRank rank, FVector loc, FRotator rot)
		: monster_id(id), rank(rank), location(loc), rotation(rot) {}
};

// End of Monster
//============================================