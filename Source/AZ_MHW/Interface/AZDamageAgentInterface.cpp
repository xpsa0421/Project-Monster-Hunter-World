// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/Interface/AZDamageAgentInterface.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_Server.h"

float IAZDamageAgentInterface::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info)
{
	// return if client
	if (!Cast<AAZGameMode_Server>(Cast<UAZGameInstance>(damaged_actor->GetWorld()->GetGameInstance())->GetGameMode()))
	{
		return 0.0f;
	}
	
	// Validity Checks
	if (!damaged_actor)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Damaged actor does not exist"));
		return 0.f;
	}
	IAZDamageAgentInterface* damaged_agent = Cast<IAZDamageAgentInterface>(damaged_actor);
	if (!damaged_agent)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Damaged actor does not inherit DamageAgentInterface"));
		return 0.f;
	}
	AAZCharacter* damaged_character = Cast<AAZCharacter>(damaged_actor);
	if (!damaged_character)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Damaged actor is not AAZCharacter"));
		return 0.f;
	}
	AAZCharacter* instigator_character = Cast<AAZCharacter>(damaged_actor);
	if (!instigator_character)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] damage_instigator is not AAZCharacter"));
		return 0.f;
	}

	// Debug log
	UE_LOG(AZMonster, Log, TEXT("[IAZDamageAgentInterface] ApplyDamage called. %s"), *attack_info.GetDebugString());

	// Process Damage on damaged actor
	return damaged_agent->ProcessDamage(Cast<AActor>(this), hit_result, attack_info);
}

float IAZDamageAgentInterface::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	// return if client
	if (!Cast<AAZGameMode_Server>(Cast<UAZGameInstance>(damage_instigator->GetWorld()->GetGameInstance())->GetGameMode()))
	{
		return 0.0f;
	}
	
	// Validity checks
	if (!damage_instigator)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Damage instigator is null!"));
		return 0.f;
	}
	const AAZCharacter* instigator_character = Cast<AAZCharacter>(damage_instigator);
	const AAZCharacter* damaged_character = Cast<AAZCharacter>(this);
	if (!instigator_character || !damaged_character)
	{
		UE_LOG(AZMonster, Warning, TEXT("[IAZDamageAgentInterface] Incorrect character is inheriting DamageAgentInterface!"));
		return 0.f;
	}
	
	// Broadcast Post Damage delegate bound functions
	if (OnTakeDamage.IsBound())
	{
		OnTakeDamage.Broadcast(damage_instigator, hit_result, attack_info);
	}
	
	// Debug log
	UE_LOG(AZMonster, Log, TEXT("[IAZDamageAgentInterface] ProcessDamage called from attack by %s to %s (%s)"),
	*instigator_character->GetName(), *damaged_character->GetName(), *attack_info.GetDebugString());
	
	return attack_info.GetDamageTotal();
}