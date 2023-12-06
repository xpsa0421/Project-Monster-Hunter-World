// Copyright Team AZ. All Rights Reserved.

#include "AZPlayer_Mannequin.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameInstance/AZGameInstance.h"


AAZPlayer_Mannequin::AAZPlayer_Mannequin()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetMesh()->SetEnableGravity(false);
	
	spring_arm_comp_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera_comp_ = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	spring_arm_comp_->SetupAttachment(GetRootComponent());
	camera_comp_->SetupAttachment(spring_arm_comp_);
	spring_arm_comp_->TargetArmLength = 250.f;
	spring_arm_comp_->bUsePawnControlRotation = false;
	camera_comp_->bUsePawnControlRotation = false;
	//camera_comp_->Activate();
}

void AAZPlayer_Mannequin::BeginPlay()
{
	Super::BeginPlay();
	game_instance_->GetPlayerController()->SetViewTarget(this);
	spring_arm_comp_->SetWorldRotation(FRotator(0,180,0));
	spring_arm_comp_->SetWorldLocation(FVector(0,75,-50));
	
}



