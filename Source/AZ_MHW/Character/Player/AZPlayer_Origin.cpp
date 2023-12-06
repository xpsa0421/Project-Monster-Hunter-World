// Copyright Team AZ. All Rights Reserved.


#include "AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_Server.h"
#include "AZ_MHW/Manager/AZObjectMgr_Server.h"
#include "Components/CapsuleComponent.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/Manager/AZInventoryManager.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "AZ_MHW/Item/AZItemData.h"
#include <Components/CapsuleComponent.h>

#include "AnimInstance/AZAnimInstance_Player.h"
#include "Character/Monster/AZMonster.h"
#include "PlayerController/AZPlayerController_Server.h"

AAZPlayer_Origin::AAZPlayer_Origin()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
}

void AAZPlayer_Origin::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	Cast<AAZGameMode_Server>(game_instance_->GetGameMode())->object_mgr_->AddObject(this);
	// Damage Interface
	OnTakeDamage.AddDynamic(this, &AAZPlayer_Origin::PostProcessDamage);
}

void AAZPlayer_Origin::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZPlayer_Origin::BeginDestroy()
{
	// Damage Interface
	OnTakeDamage.RemoveDynamic(this, &AAZPlayer_Origin::PostProcessDamage);
	Super::BeginDestroy();
}

void AAZPlayer_Origin::SetActiveSuperArmor(bool enable, int32 reduce_rate)
{
	player_character_state_->character_state_.bit_super_armor = enable;
	if(enable)
	{
		player_character_state_->character_state_.damage_reduce_rate = reduce_rate;
	}
	else
	{
		player_character_state_->character_state_.damage_reduce_rate = 0;
	}
}


float AAZPlayer_Origin::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info)
{
	attack_info.damage_array[0].amount = player_character_state_->character_state_.current_attack_point;
	return Super::ApplyDamage_Implementation(damaged_actor, hit_result, attack_info);
	
}

float AAZPlayer_Origin::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	UE_LOG(AZ, Error, TEXT("Damaged"));
	return Super::ProcessDamage(damage_instigator, hit_result, attack_info);
}

void AAZPlayer_Origin::PostProcessDamage(AActor* damage_instigator, const FHitResult hit_result,
	FAttackInfo attack_info)
{
	const auto anim_instance = Cast<UAZAnimInstance_Player>(GetMesh()->GetAnimInstance());
	if(anim_instance->current_montage_name_ == TEXT("Hit")) return;
	
	AAZMonster* instigator_monster = Cast<AAZMonster>(damage_instigator);
	if (!instigator_monster)
	{
		UE_LOG(AZMonster, Warning, TEXT("[AAZPlayer] Damage dealt by non-AZMonster actor %s"), *instigator_monster->GetName());
		return;
	}

	const float angle =GetRelativeAngleToLocation(instigator_monster->GetActorLocation());
	

	const FRotator hit_direction = FRotator(0,angle,0);
	SetActorRotation(hit_direction);
	player_character_state_->character_state_.character_direction = hit_direction;
	player_character_state_->action_state_.input_direction = hit_direction;
	
	if(player_character_state_->character_state_.bit_super_armor)
	{
		player_character_state_->character_state_.bit_hit = false;
	}
	else
	{
		player_character_state_->character_state_.bit_hit = true;
	}
	
	int32 total_damage = attack_info.GetDamageTotal();

#pragma region 거대한 기획...
	//기획상으로는
	//약,중,강의 세기로 공격받을 수 있다.
	//강일시 플레이어 캐릭터의 방향이 공격받은 방향의 크게 영향을 받고(앞, 뒤), 공격받은 방향으로의 애니메이션 처리
	//중일시 플레이어 캐릭터의 방향이 공격받은 방향의 영향을 받고(앞, 뒤), 공격받은 방향으로의 애니메이션처리
	//약일시 플레이어 캐릭터 방향은 변하지않고, 공격받은 방향으로의 애니메이션처리(일반적으로 4방향)
	//
	//monster위치, character 위치
	//const FVector damaged_position = GetActorLocation();//player 위치
	// const FVector forward_direction = GetActorForwardVector();//player 위치
	// const FVector from_hit_position = instigator_monster->GetActorLocation();//몬스터 위치
	// const FVector to_hit_direction = (from_hit_position - forward_direction).GetSafeNormal();//상대 방향
	// const double cos_theta = FVector::DotProduct(forward_direction,to_hit_direction);
	// double theta = FMath::Acos(cos_theta);
	// theta = FMath::RadiansToDegrees(theta);
	// const FVector cross_vector = FVector::CrossProduct(forward_direction,to_hit_direction);
	// if(cross_vector.Z<0)
	// {
	// 	theta *=-1.f;
	// }
	// //구르기태클2차구르기
	// //Default Back
	// if(theta >=-45.f && theta < 45.f)
	// {
	// 	//Front
	// 	UE_LOG(AZ_DAMAGE,Error,TEXT("Front"));
	// }
	// else if(theta >= -135.f && theta <-45.f)
	// {
	// 	//Left
	// 	UE_LOG(AZ_DAMAGE,Error,TEXT("Left"));
	// }
	// else if(theta >=45.f && theta < 135.f)
	// {
	// 	//Right
	// 	UE_LOG(AZ_DAMAGE,Error,TEXT("Right"));
	// }

#pragma endregion 
	
	if(const auto player_state = GetPlayerState<AAZPlayerState_Client>())
	{
		total_damage -= player_character_state_->character_state_.armor;//고정감소
		total_damage *= 1-(player_character_state_->character_state_.damage_reduce_rate/10000.f);//비율감소
		player_character_state_->character_state_.current_health_point -= total_damage;//적용
		
		if(player_state->character_state_.current_health_point <= 0)
		{
			player_state->character_state_.current_health_point = 0;
		}
		
		if(const auto& server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController()))
		{
			server_controller->Send_HitPlayer_Playable(player_character_state_->uid_,angle,total_damage);
			server_controller->BroadCast_HitPlayer_Remotable(player_character_state_->uid_,angle,total_damage);
		}
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player Health: %d"),
				player_state->character_state_.current_health_point));
		}
	}

	
}

void AAZPlayer_Origin::AnimNotify_OnUseItem()
{
	//서버처리
	//FIXME
	//정보따로,
	//사용갯수처리 따로?
	//game_cache_info_에서 잘못된 주소 오류나옴.
	//const auto& buff = game_instance_->game_cache_info_->GetCurrentCharacterItem()->UsePotion(0);

	//TEMP
	FBuffDataStruct tempbuff;
	tempbuff.calc = ECalculation::plus;
	tempbuff.target = EItemTarget::health;
	tempbuff.effect = 30;
	
	switch (tempbuff.target)
	{
	case EItemTarget::health:
		if(auto player_state = GetPlayerState<AAZPlayerState_Client>())
		{
			int32 result_health_point = 0;
			switch (tempbuff.calc)
			{
			case ECalculation::plus:
				{
					result_health_point = player_state->character_state_.current_health_point + (int)tempbuff.effect;
					if(player_state->character_state_.max_health_point < result_health_point)
					{
						player_state->character_state_.current_health_point = player_state->character_state_.max_health_point;
					}
					else
					{
						player_state->character_state_.current_health_point = result_health_point;
					}
					
					int client_index = player_character_state_->uid_;
					const auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());
					server_controller->BroadCast_UpdatePlayerState_Remotable(client_index,0,result_health_point,0);
					server_controller->Send_UpdatePlayerState_Playable(client_index,0,result_health_point,0);
				}
				break;
			case ECalculation::multi:
				break;
			}
		}
		break;
	case EItemTarget::damage:
		if(auto player_state = GetPlayerState<AAZPlayerState_Client>())
		{
			if(auto az_player_state = Cast<AAZPlayerState_Client>(player_state))
			{
				switch (tempbuff.calc)
				{
				case ECalculation::plus:
					UE_LOG(AZ_TEST,Warning,TEXT("공격력 더해짐"));
					break;
				case ECalculation::multi:
					UE_LOG(AZ_TEST,Warning,TEXT("공격력 곱해서 올라감"));
					break;
				}
			}
		}
		break;
	}
}