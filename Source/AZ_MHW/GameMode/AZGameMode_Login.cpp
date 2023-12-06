// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameMode/AZGameMode_Login.h"

#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/HUD/AZHUD_Login.h"
#include "AZ_MHW/Manager/AZGameOption.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "Kismet/GameplayStatics.h"


AAZGameMode_Login::AAZGameMode_Login()
{
	static ConstructorHelpers::FObjectFinder<USoundWave> login_bgm(TEXT("/Game/AZ/Audio/A_MainMenuTheme"));

	if (login_bgm.Succeeded())
	{
		bgm_sound_wave_ = login_bgm.Object;
	}
	game_mode_flag_ = EGameModeFlag::Login;
	DefaultPawnClass = nullptr;
	HUDClass = AAZHUD_Login::StaticClass();
}

void AAZGameMode_Login::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);
	
	game_instance_->game_option->InitGameOption();
}

void AAZGameMode_Login::BeginPlay()
{
	Super::BeginPlay();
	if (game_instance_->GetPlayerController() != nullptr)
	{
		game_instance_->GetPlayerController()->SetShowMouseCursor(true);
	}
}
