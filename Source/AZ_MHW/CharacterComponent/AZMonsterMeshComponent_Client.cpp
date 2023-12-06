// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "Engine/StaticMeshActor.h"

// Sets default values for this component's properties
UAZMonsterMeshComponent_Client::UAZMonsterMeshComponent_Client()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAZMonsterMeshComponent_Client::Init()
{
	// Set owner as monster_client
	owner_ = Cast<AAZMonster_Client>(GetOwner());
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

void UAZMonsterMeshComponent_Client::BeginPlay()
{
	Super::BeginPlay();

	// Validity checks
	if (!owner_) return;
	if (owner_->IsABoss() && mesh_material_indices_default_.IsEmpty())
	{
		UE_LOG(AZMonster, Warning, TEXT("[UAZMonsterMeshComponent_Client] Mesh material indices not set for a boss monster!"));
	}

	// Bind events and delegates
	owner_->OnBodyPartWounded.AddUObject(this, &UAZMonsterMeshComponent_Client::OnBodyPartWounded);
	owner_->OnBodyPartWoundHealed.AddUObject(this, &UAZMonsterMeshComponent_Client::OnBodyPartWoundHealed);
	owner_->OnBodyPartBroken.AddUObject(this, &UAZMonsterMeshComponent_Client::OnBodyPartBroken);
	owner_->OnBodyPartSevered.AddUObject(this, &UAZMonsterMeshComponent_Client::OnBodyPartSevered);
	owner_->OnDeath.AddDynamic(this, &UAZMonsterMeshComponent_Client::OnDeath);

	// Eye blink settings
	// Set up timer handle and delegate
	if (mesh_->GetMaterialIndex("Eyelid_Default"))
	{
		FTimerDelegate blink_eye_timer_delegate = FTimerDelegate::CreateUObject(this, &UAZMonsterMeshComponent_Client::BlinkEyes);
		owner_->GetWorld()->GetTimerManager().SetTimer(blink_eye_timer_handle_, blink_eye_timer_delegate, 4, true);
	}
}

void UAZMonsterMeshComponent_Client::SetBodyState(EMonsterBodyPart body_part, FBossBodyPartDebuffState state)
{
	if (state.is_wounded) OnBodyPartWounded(body_part);
	if (state.is_broken) OnBodyPartBroken(body_part);
	if (state.is_severed) OnBodyPartSevered(body_part);
}

void UAZMonsterMeshComponent_Client::SetUpBodyPartMaterialMaps()
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

void UAZMonsterMeshComponent_Client::InitializeMeshVisibilities()
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

void UAZMonsterMeshComponent_Client::SetUpDynamicMaterials()
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
void UAZMonsterMeshComponent_Client::SetMaterialVisibility(uint8 material_idx, bool is_visible)
{
	UMaterialInterface* original_material = mesh_->GetMaterial(material_idx);
	if (!original_material) return;
	UMaterialInstanceDynamic* dynamic_material = Cast<UMaterialInstanceDynamic>(original_material);
	if (!dynamic_material) return;
	dynamic_material->SetScalarParameterValue(FName("Opacity"), is_visible == 1 ? 1.0f : 0.0f);
}

void UAZMonsterMeshComponent_Client::SetMaterialVisibility(FName slot_name, bool is_visible)
{
	SetMaterialVisibility(mesh_->GetMaterialIndex(slot_name), is_visible);
}

void UAZMonsterMeshComponent_Client::OnBodyPartWounded(EMonsterBodyPart body_part)
{
	if (!owner_) return;
	
	SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), false);
	SetMaterialVisibility(*mesh_material_indices_wounded_.Find(body_part), true);
	//TODO Add animation
}

void UAZMonsterMeshComponent_Client::OnBodyPartWoundHealed(EMonsterBodyPart body_part)
{
	if (!owner_) return;
	
	SetMaterialVisibility(*mesh_material_indices_wounded_.Find(body_part), false);
	SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), true);
}

void UAZMonsterMeshComponent_Client::OnBodyPartBroken(EMonsterBodyPart body_part)
{
	if (!owner_) return;
	//TEMP
	if (body_part == EMonsterBodyPart::Back) return;

	SetMaterialVisibility(*mesh_material_indices_default_.Find(body_part), false);
	float visibility;
	auto wounded_mat = mesh_->GetMaterial(*mesh_material_indices_wounded_.Find(body_part));
	wounded_mat->GetScalarParameterValue(FName("Opacity"), visibility);
	if (visibility == 1)
	{
		SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), false);
	}
	
	//TODO Add animation
}

void UAZMonsterMeshComponent_Client::OnBodyPartSevered(EMonsterBodyPart body_part)
{
	SetMaterialVisibility(*mesh_material_indices_default_.Find(body_part), false);
	SetMaterialVisibility(*mesh_material_indices_cutsurface_.Find(body_part), true);

	//TODO Add animation

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

void UAZMonsterMeshComponent_Client::OnDeath()
{
	CloseEyes(true);
	GetWorld()->GetTimerManager().ClearTimer(blink_eye_timer_handle_);
}

void UAZMonsterMeshComponent_Client::CloseEyes(bool should_close)
{
	eyes_closed_ = should_close;
	SetMaterialVisibility(FName("Eyelid_Default"), should_close);
}

// Close eyes, wait for 0.2 seconds, the open 
void UAZMonsterMeshComponent_Client::BlinkEyes()
{
	// do not blink if blinded
	if (eyes_closed_) return;
	
	CloseEyes(true);
	FTimerHandle open_eyes_timer_handle;
	FTimerDelegate open_eyes_timer_delegate = FTimerDelegate::CreateUObject(this, &UAZMonsterMeshComponent_Client::CloseEyes, false);
	GetWorld()->GetTimerManager().SetTimer(open_eyes_timer_handle, open_eyes_timer_delegate, 0.17, false);
}
