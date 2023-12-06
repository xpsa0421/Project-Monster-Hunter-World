// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_Splash.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Splash : public UAZWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* animation) override;

private:
	UWidgetAnimation* splash_ani;
};
