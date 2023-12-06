// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController.h"

#include "EnhancedInputComponent.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/HUD/AZHUD.h"


AAZPlayerController::AAZPlayerController()
{
	
}

void AAZPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//?왜 여기선 GetWorld를 못하는가
	////액터가 완전히 안만들어져서 World에 대한 무언가가 없다?
	//게임 모드 로드중 월드 생성도중에 생성되는 액터라서 GetWorld가 안되는가?
	//GEngine->GetWorld()도 불가능.
}

void AAZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	
	UAZInputMgr* input_mgr = game_instance_->input_mgr_;
	input_mgr->SetupDefaultBindAction(InputComponent, GetLocalPlayer());
	input_mgr->AddInputMappingContext(TEXT("UI"));
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//TAP
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("ChangeControlMode"), ETriggerEvent::Triggered, this, &AAZPlayerController::ActionChangeControlMode);
		// BackSlash
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("CloseScene"), ETriggerEvent::Triggered, this, &AAZPlayerController::ActionCloseScene);
	}
}


void AAZPlayerController::OnPossess(APawn* in_pawn)
{
	Super::OnPossess(in_pawn);
	
}

void AAZPlayerController::BeginDestroy()
{
	Super::BeginDestroy();
}


void AAZPlayerController::ActionChangeControlMode()
{
	//TODO: UIMode <-> InGameMode
	FViewport* viewport = GetLocalPlayer()->ViewportClient->Viewport;
	
	if(bShowMouseCursor)
	{
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;

		viewport->CaptureMouse(true);
	}
	else
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		
		viewport->CaptureMouse(false);
	}
}

void AAZPlayerController::ActionCloseScene()
{
	auto current_hud = Cast<AAZHUD>(GetHUD());
	if (current_hud == nullptr)
	{
		return;
	}
	current_hud->CloseScene(true);
}



