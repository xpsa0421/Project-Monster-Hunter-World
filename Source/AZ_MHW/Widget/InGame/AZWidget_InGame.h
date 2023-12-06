// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_InGame.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_InGame : public UAZWidget
{
	GENERATED_BODY()
public:
	UAZWidget_InGame();
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void OnClose(bool immediately = false) override;
	virtual void Update() override;
	virtual void ForceHide() override;
	virtual void RestoreFromForceHidden() override;
};
