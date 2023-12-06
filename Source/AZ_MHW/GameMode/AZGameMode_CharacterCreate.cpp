// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameMode/AZGameMode_CharacterCreate.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/HUD/AZHUD_CharacterCreate.h"
#include "PlayerState/AZPlayerState_Client.h"

AAZGameMode_CharacterCreate::AAZGameMode_CharacterCreate()
{
	game_mode_flag_ = EGameModeFlag::CharacterCreate;
	HUDClass = AAZHUD_CharacterCreate::StaticClass();
	PlayerStateClass = AAZPlayerState_Client::StaticClass();
	DefaultPawnClass = nullptr;
}

void AAZGameMode_CharacterCreate::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);
}

void AAZGameMode_CharacterCreate::BeginPlay()
{
	Super::BeginPlay();
	if (game_instance_->GetPlayerController() != nullptr)
	{
		game_instance_->GetPlayerController()->SetShowMouseCursor(true);
	}
}
