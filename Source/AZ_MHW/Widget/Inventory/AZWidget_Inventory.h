// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget/AZWidget.h"
#include "Components/Button.h"
#include "AZWidget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Inventory : public UAZWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void OnClose(bool immediately = false) override;
	virtual void EditorOnly_Refresh() override;
	virtual void Update() override;
	virtual bool DifferenceBackButton() override;
	virtual void OnTouchEmptySpace() override;
	virtual void OnAlphaTimeFinished() override;
	virtual void ForceHide() override;
	virtual void RestoreFromForceHidden() override;

public:
	UPROPERTY(meta = (BindWidget)) UButton* item_check_button_;
	UPROPERTY(meta = (BindWidget)) UButton* item_move_button_;
	UPROPERTY(meta = (BindWidget)) UButton* item_equip_check_button_;
	UPROPERTY(meta = (BindWidget)) UButton* item_equip_move_button_;

public:
	UFUNCTION() void OnItemCheck();
	UFUNCTION() void OnItemMove();
	UFUNCTION() void OnEquipCheck();
	UFUNCTION() void OnEquipMove();

public:
	UAZWidget* item_check_;
	UAZWidget* item_move_;
	UAZWidget* equip_check_;
	UAZWidget* equip_move_;
};
