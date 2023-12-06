// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/StartMenu/AZWidget_Menu.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "Components/Button.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "ShaderPipelineCache.h"
#include "Kismet/GameplayStatics.h"

void UAZWidget_Menu::Init()
{
	Super::Init();

	AZ_PRINT_LOG_IF_FALSE(c_btn_start_);
	c_btn_start_->OnClicked.RemoveDynamic(this, &UAZWidget_Menu::OnClicked_Start);
	c_btn_start_->OnClicked.AddDynamic(this, &UAZWidget_Menu::OnClicked_Start);

	AZ_PRINT_LOG_IF_FALSE(c_btn_option_);
	c_btn_option_->OnClicked.RemoveDynamic(this, &UAZWidget_Menu::OnClicked_Option);
	c_btn_option_->OnClicked.AddDynamic(this, &UAZWidget_Menu::OnClicked_Option);

	AZ_PRINT_LOG_IF_FALSE(c_btn_credits_);
	c_btn_credits_->OnClicked.RemoveDynamic(this, &UAZWidget_Menu::OnClicked_Credits);
	c_btn_credits_->OnClicked.AddDynamic(this, &UAZWidget_Menu::OnClicked_Credits);

	AZ_PRINT_LOG_IF_FALSE(c_btn_exit_);
	c_btn_exit_->OnClicked.RemoveDynamic(this, &UAZWidget_Menu::OnClicked_Close);
	c_btn_exit_->OnClicked.AddDynamic(this, &UAZWidget_Menu::OnClicked_Close);
}

void UAZWidget_Menu::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
}

void UAZWidget_Menu::OnClicked_Start()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/ServerDebug/ServerDebug_Level"),true,"?game=/Game/AZ/GameMode/BP_InGame.BP_InGame_C");
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_CharacterSelect"));
}

void UAZWidget_Menu::OnClicked_Option()
{
}

void UAZWidget_Menu::OnClicked_Credits()
{
}

void UAZWidget_Menu::OnClicked_Close()
{
	//로직 통일
	game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::GameExit);
}
