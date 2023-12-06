// Copyright Team AZ. All Rights Reserved.


#include "Character/Monster/AZMonster_Model.h"


void AAZMonster_Model::BeginPlay()
{
	Super::BeginPlay();

	auto material_idx = GetMesh()->GetMaterialIndex(FName("Eyelid_Default"));
	auto eyelid_material = GetMesh()->GetMaterial(material_idx);
	eyelid_material_ = UMaterialInstanceDynamic::Create(eyelid_material, this);
	GetMesh()->SetMaterial(material_idx, eyelid_material_);
	CloseEyes(false);
	
	FTimerDelegate blink_eye_timer_delegate = FTimerDelegate::CreateUObject(this, &AAZMonster_Model::BlinkEyes);
	GetWorld()->GetTimerManager().SetTimer(blink_eye_timer_handle_, blink_eye_timer_delegate, 4, true);
}

// Close eyes, wait, then open 
void AAZMonster_Model::BlinkEyes()
{
	// do not blink if blinded
	if (eyes_closed_) return;
	
	CloseEyes(true);
	FTimerHandle open_eyes_timer_handle;
	FTimerDelegate open_eyes_timer_delegate = FTimerDelegate::CreateUObject(this, &AAZMonster_Model::CloseEyes, false);
	GetWorld()->GetTimerManager().SetTimer(open_eyes_timer_handle, open_eyes_timer_delegate, 0.17, false);
}

void AAZMonster_Model::CloseEyes(bool should_close)
{
	eyes_closed_ = should_close;
	SetMaterialVisibility(should_close);
}

/* Creates a dynamic material instance and set its opacity value */
void AAZMonster_Model::SetMaterialVisibility(bool is_visible)
{
	UMaterialInstanceDynamic* dynamic_material = Cast<UMaterialInstanceDynamic>(eyelid_material_);
	if (!dynamic_material) return;
	dynamic_material->SetScalarParameterValue(FName("Opacity"), is_visible == 1 ? 1.0f : 0.0f);
}