// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/Character/Monster/AZMonster_Client.h"

#include "AIController.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/CharacterComponent/AZMonsterPacketHandlerComponent_Client.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "Components/CapsuleComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameInstance/AZGameInstance.h"
#include "Manager/AZResourceMgr.h"

AAZMonster_Client::AAZMonster_Client()
{
	// Initialise common properties
	monster_id_ = -1;

	// Disable AI Controller
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::Disabled;
		
	// Create components
	mesh_component_ = CreateDefaultSubobject<UAZMonsterMeshComponent_Client>(TEXT("Mesh Component"));
	packet_handler_component_ = CreateDefaultSubobject<UAZMonsterPacketHandlerComponent_Client>(TEXT("Packet Handler Component"));
}

void AAZMonster_Client::Init(int32 monster_id, EBossRank rank)
{
	if (monster_id == -1) return;

	monster_id_ = monster_id;
	rank_ = rank;
	name_ = UAZGameSingleton::instance()->monster_mgr_->GetMonsterName(monster_id_);
	SetMeshAndColliders();
	mesh_component_->Init();
	packet_handler_component_->Init();
}

void AAZMonster_Client::SetMeshAndColliders()
{
	FString name = name_.ToString();
	
	// Set capsule collider
	GetCapsuleComponent()->SetCapsuleRadius(185.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(150.0f); 
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AZMonsterCapsule"));
	
	// Set skeletal mesh
	FString sk_path = FString::Printf(TEXT("/Game/AZ/Monsters/%s/Meshes/SK_%s"), *name, *name);
	if(auto sk_asset = LoadObject<USkeletalMesh>(nullptr,*sk_path))
	{
		GetMesh()->SetSkeletalMesh(sk_asset);
		GetMesh()->SetRelativeLocation(FVector(73, 0, -180));
		GetMesh()->SetCollisionProfileName(TEXT("AZMonster"));
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("[AAZMonster_Client][%d] Skeletal mesh not found!"), monster_id_);
		return;
	}

	// Set animation class
	FString anim_path = FString::Printf(TEXT("/Game/AZ/Monsters/%s/Animations/ABP_%s"), *name, *name);
	if (UClass* anim_asset = AZResourceHelper::LoadClass<UAnimInstance>(anim_path))
	{
		GetMesh()->SetAnimInstanceClass(anim_asset);
		anim_instance_ = Cast<UAZAnimInstance_Monster>(GetMesh()->GetAnimInstance());
		anim_instance_->owner_monster_client_ = this;
		anim_instance_->is_server_ = false;
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("[AAZMonster_Client][%d] ABP not found!"), monster_id_);
	}
}

void AAZMonster_Client::BeginPlay()
{
	Super::BeginPlay();
	OnDeath.AddDynamic(this, &AAZMonster_Client::ProcessDeath);
}

bool AAZMonster_Client::IsABoss() const
{
	return monster_id_ != -1 && rank_ != EBossRank::None;
}

void AAZMonster_Client::SetActionStateInfo(const FMonsterActionStateInfo action_state_info)
{
	action_state_info_ = action_state_info;
}

void AAZMonster_Client::AnimNotify_SetMovementMode(EMovementMode movement_mode)
{
	GetCharacterMovement()->SetMovementMode(movement_mode);
}

void AAZMonster_Client::ProcessDeath()
{
	action_state_info_.priority_score = EMonsterActionPriority::Death;
	anim_instance_->UpdateAnimation();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
