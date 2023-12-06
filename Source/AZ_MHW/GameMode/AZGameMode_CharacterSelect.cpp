// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameMode/AZGameMode_CharacterSelect.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/HUD/AZHUD_CharacterSelect.h"
#include "PlayerState/AZPlayerState_Client.h"

AAZGameMode_CharacterSelect::AAZGameMode_CharacterSelect()
{
	game_mode_flag_ = EGameModeFlag::CharacterCreate;
	HUDClass = AAZHUD_CharacterSelect::StaticClass();
	PlayerStateClass = AAZPlayerState_Client::StaticClass();
	DefaultPawnClass = nullptr;
}

void AAZGameMode_CharacterSelect::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);
}

void AAZGameMode_CharacterSelect::BeginPlay()
{
	Super::BeginPlay();
	if (game_instance_->GetPlayerController() != nullptr)
	{
		game_instance_->GetPlayerController()->SetShowMouseCursor(true);
	}
}
