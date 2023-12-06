// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameMode/AZGameMode_Launcher.h"

#include "AZ_MHW/HUD/AZHUD_Launcher.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameOption.h"
#include "AZ_MHW/PlayerState/AZGameState_Client.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"

AAZGameMode_Launcher::AAZGameMode_Launcher()
{
	game_mode_flag_ = EGameModeFlag::Launcher;

	DefaultPawnClass = nullptr;//아직까지는 인게임이 아니기 떄문에

	//PlayerControllerClass = ?//아직까지는 인게임이 아니기 때문에
	PlayerStateClass = AAZPlayerState_Client::StaticClass();
	GameStateClass = AAZGameState_Client::StaticClass();

	HUDClass = AAZHUD_Launcher::StaticClass();
}

void AAZGameMode_Launcher::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);

	game_instance_->game_option->InitGameOption();
}