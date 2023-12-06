// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterAggroComponent.h"
#include "AZ_MHW/Character/Player/AZPlayer_Origin.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameMode/AZGameMode_Server.h"
#include "AZ_MHW/Manager/AZObjectMgr_Server.h"
#include "Kismet/KismetSystemLibrary.h"

UAZMonsterAggroComponent::UAZMonsterAggroComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	inrange_update_rate_ = 20.0f;
	target_serial_ = INDEX_NONE;
	target_ref_ = nullptr;
}

void UAZMonsterAggroComponent::Init()
{
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_)
	{
		UE_LOG(AZMonster_Aggro, Error, TEXT("[AZMonsterAggroComponent] Invalid owner actor!"));
	}
	percept_range_ = owner_->percept_radius_;
}

void UAZMonsterAggroComponent::ActivateSystem()
{
	UE_LOG(AZMonster_Aggro, Warning, TEXT("[AZMonsterAggroComponent] Activated"));
	FTimerDelegate timer_delegate;
	timer_delegate.BindUObject(this, &UAZMonsterAggroComponent::UpdateByRange);
	owner_->GetWorld()->GetTimerManager().SetTimer(inrange_update_timer_handle_, timer_delegate, inrange_update_rate_, true);
}

void UAZMonsterAggroComponent::InactivateSystem()
{
	UE_LOG(AZMonster_Aggro, Warning, TEXT("[AZMonsterAggroComponent] Inactivated"));
	if (owner_->GetWorld()->GetTimerManager().IsTimerActive(inrange_update_timer_handle_))
	{
		owner_->GetWorld()->GetTimerManager().ClearTimer(inrange_update_timer_handle_);
	}
}

void UAZMonsterAggroComponent::ActivateByEnterCombat(int32 player_serial)
{
	ActivateSystem();
	UpdateAggroSpecific(player_serial, 30, "EnterCombat");
	UpdateBestTarget();
}

void UAZMonsterAggroComponent::Reset()
{
	target_serial_ = INDEX_NONE;
	target_ref_ = nullptr;
	aggro_map_.Empty();
	InactivateSystem();
}

void UAZMonsterAggroComponent::BeginPlay()
{
	Super::BeginPlay();
	//owner_->OnDeath.AddDynamic(this, &UAZMonsterAggroComponent::InactivateSystem);
}

void UAZMonsterAggroComponent::ForceSetBestTarget(AAZPlayer_Origin* character)
{
	target_serial_ = character->object_serial_;
	target_ref_ = character;
}

int32 UAZMonsterAggroComponent::GetTargetSerial()
{
	if (aggro_map_.IsEmpty()) return INDEX_NONE;
	return target_serial_;
}

AAZPlayer_Origin* UAZMonsterAggroComponent::GetTargetRef() 
{
	return Cast<AAZPlayer_Origin>(target_ref_);
}

FVector UAZMonsterAggroComponent::GetTargetLocation()  
{
	if (!target_ref_)
		return FVector::ZeroVector;
	else
		return target_ref_->GetActorLocation();
}

FVector UAZMonsterAggroComponent::GetRandomTargetLocation()
{
	const int rand_idx = FMath::RandRange(0, aggro_map_.Num()-1);
	auto object_mgr = Cast<AAZGameMode_Server>(Cast<UAZGameInstance>(owner_->GetWorld()->GetGameInstance())->GetGameMode())->object_mgr_;
	int32 rand_player_idx = aggro_map_.Array()[rand_idx].Key;
	return object_mgr->GetPlayer(rand_player_idx)->GetActorLocation();
}

float UAZMonsterAggroComponent::GetDistance2DToTarget() 
{
	if (!target_ref_)
		return 0.0f;
	else
		return FVector::Dist2D(owner_->GetActorLocation(), GetTargetLocation());
}

float UAZMonsterAggroComponent::GetAngle2DToTarget() 
{
	if (!target_ref_)
		return 0.0f;
	else
		return owner_->GetRelativeAngleToLocation(GetTargetLocation());
}

void UAZMonsterAggroComponent::UpdateBestTarget()
{
	if (aggro_map_.IsEmpty())
	{
		target_serial_ = INDEX_NONE;
		target_ref_ = nullptr;
		return;
	}

	TArray<int32> best_target_serials;
	int32 highest_aggro_point = -1;
	for (auto pair : aggro_map_)
	{
		if (pair.Value > highest_aggro_point)
		{
			best_target_serials.Empty();
			best_target_serials.Add(pair.Key);
			highest_aggro_point = pair.Value;
		}
		else if (pair.Value == highest_aggro_point)
		{
			best_target_serials.Add(pair.Key);
		}
	}

	int32 prev_best_target = target_serial_;
	if (best_target_serials.Num() == 1)
	{
		target_serial_ = best_target_serials[0];
	}
	else if (best_target_serials.Num() > 1)
	{
		target_serial_ = best_target_serials[FMath::RandRange(0, best_target_serials.Num()-1)];
	}

	target_ref_ = Cast<AAZGameMode_Server>(Cast<UAZGameInstance>(owner_->GetWorld()->GetGameInstance())->GetGameMode())
		->object_mgr_->GetPlayer(target_serial_);
	if (prev_best_target != target_serial_)
		UE_LOG(AZMonster_Aggro, Log, TEXT("[UAZMonsterAggroComponent][%d] Best aggro target updated to player %d"),	owner_->object_serial_, target_serial_);
}

void UAZMonsterAggroComponent::UpdateAggroSpecific(int32 player_serial, int32 aggro_point, FString update_reason)
{
	if (auto current_point = aggro_map_.Find(player_serial))
	{
		int32 new_point = *current_point += aggro_point;
		if (new_point <= 0)
		{
			RemoveTarget(player_serial);
		}
		else
		{
			aggro_map_.Emplace(player_serial, new_point);
			UE_LOG(AZMonster_Aggro, Log, TEXT("[UAZMonsterAggroComponent][%d] Player %d aggro updated by %d due to [%s]. Current point: %d"),
				owner_->object_serial_, player_serial, aggro_point, *update_reason, *current_point);
		}
	}
	else
	{
		if (aggro_point <= 0) return;
		aggro_map_.Emplace(player_serial, aggro_point);
		UE_LOG(AZMonster_Aggro, Warning, TEXT("[UAZMonsterAggroComponent][%d] Player %d added to aggro list due to [%s]. Current point: %d"),
			owner_->object_serial_, player_serial, *update_reason, aggro_point);
	}
}

void UAZMonsterAggroComponent::RemoveTarget(int32 player_serial)
{
	if (!aggro_map_.Find(player_serial)) return;
	aggro_map_.Remove(player_serial);
	UE_LOG(AZMonster_Aggro, Log, TEXT("[UAZMonsterAggroComponent][%d] Player %d removed from the aggro list!"),
		owner_->object_serial_, player_serial);
	if (aggro_map_.IsEmpty()) UpdateBestTarget();
}

void UAZMonsterAggroComponent::IncreaseByDamage(int32 attacker_serial, int32 damage)
{
	UpdateAggroSpecific(attacker_serial, damage * 0.4, TEXT("Damage"));
}

void UAZMonsterAggroComponent::UpdateByRange()
{
	// Do sphere overlap to find players in range
	TArray<AActor*, FDefaultAllocator> ignore_actors;
	TArray<AActor*> actors_in_range;
	ignore_actors.Add(owner_.Get());

#ifdef WITH_EDITOR
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), owner_->GetActorLocation(), percept_range_,
		owner_->hit_object_types_, AAZPlayer_Origin::StaticClass(), ignore_actors, actors_in_range);
	UKismetSystemLibrary::DrawDebugSphere(owner_.Get(), owner_->GetActorLocation(), percept_range_, 24, FLinearColor::Gray,
		1.0f, 4.0f);
#endif
	
	// Get serial number of actors in range
	// Increase aggro point for players in range
	TArray<int32> serials_in_range;
	for (auto actor : actors_in_range)
	{
		if (auto player = Cast<AAZPlayer_Origin>(actor))
		{
			serials_in_range.Add(player->object_serial_);
			UpdateAggroSpecific(player->object_serial_, 5, "InRange");
		}
	}
	if (serials_in_range.IsEmpty()) return;

	// Decrease aggro point for players not in range
	for (auto info : aggro_map_)
	{
		if (!serials_in_range.Contains(info.Key))
		{
			UpdateAggroSpecific(info.Key, -30, "NotInRange");
		}
	}
}

void UAZMonsterAggroComponent::IncreaseByPartChange(int32 attacker_serial, EMonsterBodyPartChangeType change_type)
{
	switch (change_type)
	{
		case (EMonsterBodyPartChangeType::Wound) :
		{
			UpdateAggroSpecific(attacker_serial, 20, "PartWound");
			break;
		}
		case (EMonsterBodyPartChangeType::Break) :
		{
			UpdateAggroSpecific(attacker_serial, 50, "PartBreak");
			break;
		}
		case (EMonsterBodyPartChangeType::Sever) :
		{
			UpdateAggroSpecific(attacker_serial, 100, "PartSever");
			break;
		}
	}
}

// TODO Remove player on leave combat map
