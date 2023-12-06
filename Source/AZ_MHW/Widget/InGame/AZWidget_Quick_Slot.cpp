// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/InGame/AZWidget_Quick_Slot.h"



void UAZWidget_Quick_Slot::Init()
{
	Super::Init();
	quick_use_slot_0_->Init();
	quick_use_slot_1_->Init();
	quick_use_slot_2_->Init();
	quick_slot_count_->SetText(FText::FromString(L""));
}

void UAZWidget_Quick_Slot::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
}

void UAZWidget_Quick_Slot::OnClose(bool immediately)
{
	Super::OnClose(immediately);
}

void UAZWidget_Quick_Slot::Update()
{
	Super::Update();
}

void UAZWidget_Quick_Slot::ForceHide()
{
	Super::ForceHide();
	SetVisibility(ESlateVisibility::Hidden);
}

void UAZWidget_Quick_Slot::RestoreFromForceHidden()
{
	Super::RestoreFromForceHidden();
	SetVisibility(ESlateVisibility::Visible);
}
