// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Widget/InGame/AZWidget_InGame.h"

UAZWidget_InGame::UAZWidget_InGame()
{
}

void UAZWidget_InGame::Init()
{
}

void UAZWidget_InGame::OnOpen(bool immediately)
{
	return;
}

void UAZWidget_InGame::OnClose(bool immediately)
{
}

void UAZWidget_InGame::Update()
{
}

void UAZWidget_InGame::ForceHide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_InGame::RestoreFromForceHidden()
{
	SetVisibility(ESlateVisibility::Visible);
}
