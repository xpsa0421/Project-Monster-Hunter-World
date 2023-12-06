// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "Engine/StaticMeshActor.h"

// Sets default values for this component's properties
UAZMonsterMeshComponent::UAZMonsterMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAZMonsterMeshComponent::Init()
{
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_)
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMeshComponent] Invalid owner actor!"));
	}
	if (!owner_->GetMesh())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMeshComponent] Invalid owner mesh!"));
	}
	if (owner_->IsABoss())
	{
		SetUpBodyPartMaterialMaps();
		SetUpDynamicMaterials();
		InitializeMeshVisibilities();
	}
}

void UAZMonsterMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	// Validity checks
	if (!owner_) return;
	if (owner_->IsABoss() && mesh_material_indices_default_.IsEmpty())
	{
		UE_LOG(AZMonster, Warning, TEXT("[AZMonsterMeshComponent] Mesh material indices not set for a boss monster!"));
	}

	// Bind events and delegates
	owner_->OnBodyPartWounded.AddUObject(this, &UAZMonsterMeshComponent::OnBodyPartWounded);
	owner_->OnBodyPartWoundHealed.AddUObject(this, &UAZMonsterMeshComponent::OnBodyPartWoundHealed);
	owner_->OnBodyPartBroken.AddUObject(this, &UAZMonsterMeshComponent::OnBodyPartBroken);
	owner_->OnBodyPartSevered.AddUObject(this, &UAZMonsterMeshComponent::OnBodyPartSevered);
	owner_->OnDeath.AddDynamic(this, &UAZMonsterMeshComponent::OnDeath);

	// Eye blink settings
	// Set up timer handle and delegate
	if (mesh_->GetMaterialIndex("Eyelid_Default"))
	{
		FTimerDelegate blink_eye_timer_delegate = FTimerDelegate::CreateUObject(this, &UAZMonsterMeshComponent::BlinkEyes);
		GetWorld()->GetTimerManager().SetTimer(blink_eye_timer_handle_, blink_eye_timer_delegate, 4, true);
	}
}

void UAZMonsterMeshComponent::SetUpBodyPartMaterialMaps()
{
	mesh_ = owner_->GetMesh();

	int32 material_idx = 0;
	TArray<FName> material_slot_names = owner_->GetMesh()->GetMaterialSlotNames();
	for (FName slot_name : material_slot_names)
	{
		// Split the material slot name into body part name and mesh type
		FString slot_name_str = slot_name.ToString();
		FString body_part_str, mesh_type_str;
		if (!slot_name_str.Split(TEXT("_"), &body_part_str, &mesh_type_str))
		{
			UE_LOG(AZMonster, Warning, TEXT("[AZMonsterMeshComponent] Material slot name <%s> is in incorrect format"), *slot_name_str);
			material_idx++;
			continue;
		}

		// Add the body part name and material index to the respective material map
		if (mesh_type_str.Equals("Default", ESearchCase::IgnoreCase))
		{
			mesh_material_indices_default_.Add(UAZUtility::StringToEnum<EMonsterBodyPart>(body_part_str), material_idx);
		}
		else if (mesh_type_str.Equals("Wounded", ESearchCase::IgnoreCase))
		{
			mesh_material_indices_wounded_.Add(UAZUtility::StringToEnum<EMonsterBodyPart>(body_part_str), material_idx);
		}
		else if (mesh_type_str.Equals("CutSurface", ESearchCase::IgnoreCase))
        {
        	mesh_material_indices_cutsurface_.Add(UAZUtility::StringToEnum<EMonsterBodyPart>(body_part_str), material_idx);
        }
		else
		{
			UE_LOG(AZMonster, Warning, TEXT("[AZMonsterMeshComponent] Body part mesh type <%s> is in incorrect format"), *mesh_type_str);
		}
		material_idx++;
	}
}

void UAZMonsterMeshComponent::InitializeMeshVisibilities()
{
	if (!owner_) return;
	
	// Set up initial material opacities
	// Hide all damaged meshes
	for (const auto material_idx_pair : mesh_material_indices_wounded_)
	{
		SetMaterialVisibility(material_idx_pair.Value, false);
	}

	// Hide all cut surfaces
	for (const auto material_idx_pair : mesh_material_indices_cutsurface_)
	{
		SetMaterialVisibility(material_idx_pair.Value, false);
	}
	
	// Hide eyelid mesh by default
	CloseEyes(false);
}

void UAZMonsterMeshComponent::SetUpDynamicMaterials()
{
	for (int material_idx = 0; material_idx < mesh_->GetNumMaterials(); material_idx++)
	{
		UMaterialInterface* material = mesh_->GetMaterial(material_idx);
		if (!material) return;
		UMaterialInstanceDynamic* dynamic_material = UMaterialInstanceDynamic::Create(material, owner_.Get());
		mesh_->SetMaterial(material_idx, dynamic_material);
	}
}

/* Creates a dynamic material instance and set its opacity value */
void UAZMonsterMeshComponent::SetMaterialVisibility(uint8 material_idx, bool is_visible)
{
	UMaterialInterface* original_material = mesh_->GetMaterial(material_idx);
	if (!original_material) return;
	UMaterialInstanceDynamic* dynamic_material = Cast<UMaterialInstanceDynamic>(original_material);
	if (!dynamic_material) return;
	dynamic_material->SetScalarParameterValue(FName("Opacity"), is_visible == 1 ? 1.0f : 0.0f);
}

void UAZMonsterMeshComponent::SetMaterialVisibility(FName slot_name, bool is_visible)
{
	SetMaterialVisibility(mesh_->GetMaterialIndex(slot_name), is_visible);
}

void UAZMonsterMeshComponent::OnBodyPartWounded(EMonsterBodyPart body_part)
{
	if (!owner_) return;
	
	SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), false);
	SetMaterialVisibility(*mesh_material_indices_wounded_.Find(body_part), true);
	//TODO Add animation
}

void UAZMonsterMeshComponent::OnBodyPartWoundHealed(EMonsterBodyPart body_part)
{
	if (!owner_) return;
	
	SetMaterialVisibility(*mesh_material_indices_wounded_.Find(body_part), false);
	SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), true);
}

void UAZMonsterMeshComponent::OnBodyPartBroken(EMonsterBodyPart body_part)
{
	if (!owner_) return;
	//TEMP
	if (body_part == EMonsterBodyPart::Back) return;

	SetMaterialVisibility(*mesh_material_indices_default_.Find(body_part), false);
	if (!owner_->health_component_->IsWounded(body_part))
	{
		SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), false);
	}
	//TODO Add animation
}

void UAZMonsterMeshComponent::OnBodyPartSevered(EMonsterBodyPart body_part)
{
	SetMaterialVisibility(*mesh_material_indices_default_.Find(body_part), false);
	SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), true);
	
	//TODO Add animation

	// Remove collision of the severed part
	FName bone_name = (body_part == EMonsterBodyPart::Tail) ? FName("BoneFunction_144") : NAME_None; // 원래는 enum 통해서 받아와야 하지만 현재 skeleton 이름이 기본값이라 불가능
	mesh_->GetBodyInstance(bone_name)->SetShapeCollisionEnabled(0, ECollisionEnabled::NoCollision);
	
	// Drop body part mesh
	FString mesh_filepath = FString::Printf(TEXT("/Game/AZ/Monsters/%s/Meshes/SM_%s_%s.SM_%s_%s"),
		*owner_->name_.ToString(), *owner_->name_.ToString(),
		*UAZUtility::EnumToString(body_part), *owner_->name_.ToString(), *UAZUtility::EnumToString(body_part));
	UStaticMesh* mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), owner_, *mesh_filepath));
	if (mesh)
	{
		FTransform spawn_transform = mesh_->GetSocketTransform(FName(FString::Printf(TEXT("%sSocket"), *UAZUtility::EnumToString(body_part))));
		AStaticMeshActor* mesh_actor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), spawn_transform);
		if (UStaticMeshComponent* mesh_comp = mesh_actor->GetStaticMeshComponent())
		{
			mesh_comp->SetCollisionProfileName(FName("AZMonsterCapsule"));
			mesh_comp->SetMobility(EComponentMobility::Movable);
			mesh_comp->SetStaticMesh(mesh);
			mesh_comp->SetSimulatePhysics(true);
			mesh_comp->SetEnableGravity(true);
		}
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("Failed to spawn body part mesh"));
	}
}

void UAZMonsterMeshComponent::OnDeath()
{
	CloseEyes(true);
	GetWorld()->GetTimerManager().ClearTimer(blink_eye_timer_handle_);
}

void UAZMonsterMeshComponent::CloseEyes(bool should_close)
{
	eyes_closed_ = should_close;
	SetMaterialVisibility(FName("Eyelid_Default"), should_close);
}

// Close eyes, wait, then open 
void UAZMonsterMeshComponent::BlinkEyes()
{
	// do not blink if blinded
	if (eyes_closed_) return;
	
	CloseEyes(true);
	FTimerHandle open_eyes_timer_handle;
	FTimerDelegate open_eyes_timer_delegate = FTimerDelegate::CreateUObject(this, &UAZMonsterMeshComponent::CloseEyes, false);
	GetWorld()->GetTimerManager().SetTimer(open_eyes_timer_handle, open_eyes_timer_delegate, 0.17, false);
}
