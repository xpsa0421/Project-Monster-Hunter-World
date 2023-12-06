// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "Components/Image.h"
#include "AZWidget_Equipment_Slot.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Equipment_Slot : public UAZWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;

public:
	UPROPERTY(meta = (BindWidget)) UAZButton* equip_;
	UPROPERTY(meta = (BindWidget)) UImage* image_equip_;
};
