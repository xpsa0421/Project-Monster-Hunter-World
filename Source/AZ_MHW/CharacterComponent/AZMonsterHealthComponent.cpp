// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"
#include <PhysicalMaterials/PhysicalMaterial.h>

#include "AZMonsterAggroComponent.h"
#include "Character/Player/AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"

UAZMonsterHealthComponent::UAZMonsterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAZMonsterHealthComponent::Init()
{
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_)
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterHealthComponent] Invalid owner actor!"));
	}

	// Set monster properties
	if (!owner_->IsAValidMonster()) return;
	const FMonsterInfo* monster_info = UAZGameSingleton::instance()->monster_mgr_->GetMonsterInfo(owner_->GetMonsterID());
	base_hp_ = monster_info->base_hp;

	// Set boss properties
	if (!owner_->IsABoss()) return;
	FBossInfo boss_info = UAZGameSingleton::instance()->monster_mgr_->GetBossInfo(owner_->GetMonsterID(), owner_->GetBossRank());
	
	body_part_states_.Add(EMonsterBodyPart::Head, boss_info.head_state);
	body_part_states_.Add(EMonsterBodyPart::Back, boss_info.body_state);
	body_part_states_.Add(EMonsterBodyPart::LeftWing, boss_info.wing_state);
	body_part_states_.Add(EMonsterBodyPart::RightWing, boss_info.wing_state);
	body_part_states_.Add(EMonsterBodyPart::Tail, boss_info.tail_state);
	body_part_states_.Add(EMonsterBodyPart::Leg, boss_info.leg_state);
	
	tenderised_damage_multiplier_	= boss_info.tenderised_damage_multiplier;
	escape_stats_					= boss_info.escape_stats;
	weakness_stats_					= boss_info.weakness_stats;
	base_stamina_					= boss_info.base_stamina;
	tired_duration_					= boss_info.tired_duration;

	InitializeRuntimeValues();	
}

void UAZMonsterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Damage Interface
	//owner_->OnTakeDamage.AddDynamic(this, &UAZMonsterHealthComponent::PostProcessDamage);
	owner_->OnBodyPartWounded.AddUObject(this, &UAZMonsterHealthComponent::OnBodyPartWounded);
	owner_->OnBodyPartWoundHealed.AddUObject(this, &UAZMonsterHealthComponent::OnBodyPartWoundHealed);
	owner_->OnBodyPartBroken.AddUObject(this, &UAZMonsterHealthComponent::OnBodyPartBroken);
	owner_->OnBodyPartSevered.AddUObject(this, &UAZMonsterHealthComponent::OnBodyPartSevered);
	owner_->OnEnraged.AddDynamic(this, &UAZMonsterHealthComponent::OnEnraged);
	owner_->OnEnrageEnded.AddDynamic(this, &UAZMonsterHealthComponent::OnEnrageEnded);
	owner_->OnDeath.AddDynamic(this, &UAZMonsterHealthComponent::OnDeath);
}

void UAZMonsterHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAZMonsterHealthComponent::InitializeRuntimeValues()
{
	current_hp_ = base_hp_;
	current_stamina_ = base_stamina_;
	current_num_escapes_ = 0;
	wound_duration_ = 20.0f;
	rage_accumulated_damage_ = 0;
}

float UAZMonsterHealthComponent::GetHealthRatio() const
{
	return current_hp_ / base_hp_;
}

float UAZMonsterHealthComponent::GetStaminaRatio() const
{
	return current_stamina_ / base_stamina_;
}

bool UAZMonsterHealthComponent::IsWounded(EMonsterBodyPart body_part)
{
	const auto state = body_part_states_.Find(body_part);
	if (state != nullptr) return state->is_wounded;
	else return false;
}

bool UAZMonsterHealthComponent::IsSevered(EMonsterBodyPart body_part)
{
	const auto state = body_part_states_.Find(body_part);
	if (state != nullptr) return state->is_severed;
	else return false;
}

bool UAZMonsterHealthComponent::IsPendingKill() const
{
	return current_hp_ <= 0;
}

TMap<EMonsterBodyPart, FBossBodyPartState>* UAZMonsterHealthComponent::GetBodyPartStates()
{
	return &body_part_states_;
}

float UAZMonsterHealthComponent::ApplyDamage(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info)
{
	// Validity checks
	if (IsPendingKill())
	{
		return 0.f;
	}
	if (owner_->IsEnraged())
	{
		for (auto& damage_info : attack_info.damage_array)
		{
			damage_info.amount *= owner_->rage_stats_.outgoing_damage_multiplier;
		}
	}
	IAZDamageAgentInterface* damaged_agent = Cast<IAZDamageAgentInterface>(damaged_actor);
	if (!damaged_agent)
	{
		return 0.f;
	}

	// Do damage
	damaged_agent->ProcessDamage(Cast<AActor>(owner_), hit_result, attack_info);

	// Log
#ifdef WITH_EDITOR
	FString damage_types, damage_amounts;
	for (auto info : attack_info.damage_array)
	{
		damage_types += UAZUtility::EnumToString(info.type) + " ";
		damage_amounts += FString::FromInt(info.amount) + " ";
	}
	UE_LOG(AZMonster, Warning, TEXT("[UAZMonsterHealthComponent] Damage applied to %s. Type: %s, Amount: %s, Effect: %s"),
		*damaged_actor->GetName(), *damage_types, *damage_amounts, *UAZUtility::EnumToString(attack_info.attack_effect));
#endif
	
	return attack_info.GetDamageTotal();
}

float UAZMonsterHealthComponent::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	if (IsPendingKill()) return 0.f;
	
	// check if this attack is a transition trigger
	if (!owner_->IsInCombat())
	{
		owner_->EnterCombat(damage_instigator, false);
	}
	
	// Check if attacker is valid
	AAZPlayer_Origin* attacker = Cast<AAZPlayer_Origin>(damage_instigator);
	if (!attacker)
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterHealthComponent] Invalid damage instigator"));
		return 0.f;
	}
	
	// Check if damage is valid
	// Single damage type exists for each hunter's attack
	const EMonsterBodyPart damaged_body_part = static_cast<EMonsterBodyPart>(hit_result.PhysMaterial.Get()->SurfaceType.GetValue());
	const EDamageType damage_type = attack_info.damage_array[0].type;
	if (!IsReceivedAttackValid(damage_type, damaged_body_part)) return 0.f;

	// If already severed, cannot do damage
	if (IsSevered(damaged_body_part)) return 0.f;

	// TODO status effects
	EAttackEffectType attack_effect = attack_info.attack_effect;
	
	// Apply weakness
	int32 weakness = weakness_stats_.weakness_array[UAZUtility::EnumToByte(damaged_body_part)][UAZUtility::EnumToByte(damage_type)];
	float processed_damage =  attack_info.damage_array[0].amount * (weakness / 10);

	// Apply rage mode multiplier
	if (owner_->IsEnraged())
	{
	    processed_damage *= owner_->rage_stats_.incoming_damage_multiplier;
	    UE_LOG(AZMonster, Log, TEXT("[UAZMonsterHealthComponent] Damage dealt when monster is enraged! Damage is multiplied by %f"),
			owner_->rage_stats_.incoming_damage_multiplier);
	}
	
	// Apply tenderised multiplier
    if (IsWounded(damaged_body_part))
    {
        processed_damage *= tenderised_damage_multiplier_;
        UE_LOG(AZMonster, Log, TEXT("[UAZMonsterHealthComponent] Damage dealt to wounded part [%s]! Damage is multiplied by %f"),
            *UAZUtility::EnumToString(damaged_body_part), tenderised_damage_multiplier_);
    }
	
	// Body part specific damage processing
	ApplyDamageToBodyPart(damage_type, damaged_body_part, processed_damage);

	// General processing
	ReduceHealth(processed_damage);

	// Update aggro information
	owner_->aggro_component_->IncreaseByDamage(attacker->object_serial_, processed_damage);

	// Update rage information
	if (!owner_->IsEnraged())
	{
		rage_accumulated_damage_ += processed_damage;
		if (owner_->rage_stats_.required_damage <= rage_accumulated_damage_)
		{
			owner_->OnEnraged.Broadcast();
		}
	}
	
	owner_->OnHit.Broadcast(attacker, FHitResultInfo(processed_damage, hit_result.TraceStart, hit_result.TraceEnd, hit_result.Location));
	return processed_damage;
}

bool UAZMonsterHealthComponent::IsReceivedAttackValid(EDamageType damage_type, EMonsterBodyPart damaged_part)
{
	// Check if valid body part is hit
	if (damaged_part == EMonsterBodyPart::Default || damaged_part >= EMonsterBodyPart::Eyelid)
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterHealthComponent] Invalid body part: %s"), *UAZUtility::EnumToString(damaged_part));
		return false;
	}

	// Check if valid damage type is received
	if (damage_type > EDamageType::Ammo)
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterHealthComponent] Invalid damage type: %s"), *UAZUtility::EnumToString(damage_type));
		return false;
	}
	return true;
}

void UAZMonsterHealthComponent::ApplyDamageToBodyPart(EDamageType damage_type, EMonsterBodyPart damaged_part, float damage_amount)
{
    // Group nearby body parts
	EMonsterBodyPart damaged_part_group = damaged_part;
	if (damaged_part == EMonsterBodyPart::Neck) damaged_part_group = EMonsterBodyPart::Head;
	else if (damaged_part == EMonsterBodyPart::Body) damaged_part_group = EMonsterBodyPart::Back;

    // Check and apply body part state changes
	CheckBeWounded(damaged_part_group, damage_amount);
	CheckBeBroken(damaged_part_group, damage_amount);
	if (damage_type == EDamageType::Cut) CheckBeSevered(damaged_part_group, damage_amount);

	//TODO do something by damage type
}

void UAZMonsterHealthComponent::CheckBeWounded(EMonsterBodyPart damaged_part, float damage_amount)
{
	auto body_part_info = body_part_states_.Find(damaged_part);
	if (!body_part_info->is_woundable || body_part_info->is_wounded) return;

	body_part_info->wound_accumulated_damage += damage_amount;
	if (body_part_info->wound_required_damage <= body_part_info->wound_accumulated_damage)
	{
		owner_->OnBodyPartWounded.Broadcast(damaged_part);
	}
}

void UAZMonsterHealthComponent::OnBodyPartWounded(EMonsterBodyPart body_part)
{
    UE_LOG(AZMonster, Log, TEXT("[UAZMonsterHealthComponent] [#%d] %s wounded)"), owner_->GetMonsterID(), *UAZUtility::EnumToString(body_part));
    
    // Set wound timer
    FTimerHandle wound_timer;
    GetWorld()->GetTimerManager().SetTimer(wound_timer, FTimerDelegate::CreateLambda([this, body_part]() {
        owner_->OnBodyPartWoundHealed.Broadcast(body_part);}), wound_duration_, false);
	
	auto body_part_info = body_part_states_.Find(body_part);
	body_part_info->is_wounded = true;
	body_part_info->wound_accumulated_damage = 0;
}

void UAZMonsterHealthComponent::OnBodyPartWoundHealed(EMonsterBodyPart body_part)
{
	UE_LOG(AZMonster, Log, TEXT("[UAZMonsterHealthComponent] [#%d] %s wound healed)"), owner_->GetMonsterID(), *UAZUtility::EnumToString(body_part));
	auto body_part_info = body_part_states_.Find(body_part);
	body_part_info->is_wounded = false;
	body_part_info->wound_accumulated_damage = 0;
}

void UAZMonsterHealthComponent::CheckBeBroken(EMonsterBodyPart damaged_part, float damage_amount)
{
	auto body_part_info = body_part_states_.Find(damaged_part);
	if (!body_part_info->is_breakable || body_part_info->is_broken) return;

	body_part_info->break_accumulated_damage += damage_amount;
	if (body_part_info->break_required_damage <= body_part_info->break_accumulated_damage)
	{
		owner_->OnBodyPartBroken.Broadcast(damaged_part);
	}
}

void UAZMonsterHealthComponent::OnBodyPartBroken(EMonsterBodyPart body_part)
{
	UE_LOG(AZMonster, Log, TEXT("[UAZMonsterHealthComponent] [#%d] %s broken)"), owner_->GetMonsterID(), *UAZUtility::EnumToString(body_part));
	auto body_part_info = body_part_states_.Find(body_part);
	body_part_info->is_broken = true;
	body_part_info->break_accumulated_damage = 0;
}

void UAZMonsterHealthComponent::CheckBeSevered(EMonsterBodyPart damaged_part, float damage_amount)
{
	auto body_part_info = body_part_states_.Find(damaged_part);
	if (!body_part_info->is_severable || body_part_info->is_severed) return;

	body_part_info->sever_accumulated_damage += damage_amount;
	if (body_part_info->sever_required_damage <= body_part_info->sever_accumulated_damage)
	{
		owner_->OnBodyPartSevered.Broadcast(damaged_part);
	}
}

void UAZMonsterHealthComponent::OnBodyPartSevered(EMonsterBodyPart body_part)
{
	UE_LOG(AZMonster, Log, TEXT("[UAZMonsterHealthComponent] [#%d] %s severed)"), owner_->GetMonsterID(), *UAZUtility::EnumToString(body_part));
	auto body_part_info = body_part_states_.Find(body_part);
	body_part_info->is_severed = true;
	body_part_info->sever_accumulated_damage = 0;
}

void UAZMonsterHealthComponent::OnEnraged()
{
	rage_accumulated_damage_ = 0;
	owner_->SetEnraged(true);
	FTimerHandle rage_timer_handle;
	owner_->GetWorld()->GetTimerManager().SetTimer(
		rage_timer_handle, FTimerDelegate::CreateUObject(this, &UAZMonsterHealthComponent::OnEnrageEnded),
		owner_->rage_stats_.duration, false);
}

void UAZMonsterHealthComponent::OnEnrageEnded()
{
	owner_->SetEnraged(false);
	owner_->OnEnrageEnded.Broadcast();
}

void UAZMonsterHealthComponent::ReduceHealth(float amount)
{
	if (IsPendingKill()) return;
	
	current_hp_ = FMath::Clamp(current_hp_ -= amount, 0, base_hp_);
	if (current_hp_ <= 0)
	{
		owner_->OnDeath.Broadcast();
	};
}

void UAZMonsterHealthComponent::OnDeath()
{
	UE_LOG(AZMonster, Warning, TEXT("[UAZMonsterHealthComponent] Kill Called"));
}
