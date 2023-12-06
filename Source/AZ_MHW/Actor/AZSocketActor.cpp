// Copyright Team AZ. All Rights Reserved.


#include "AZSocketActor.h"

AAZSocketActor::AAZSocketActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	socket_mesh_asset_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SocketItem"));
	socket_mesh_asset_->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(socket_mesh_asset_);
	socket_fx_mesh_asset_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Socket_Item_FX"));
	socket_fx_mesh_asset_->SetCollisionProfileName(TEXT("NoCollision"));
	socket_fx_mesh_asset_->SetupAttachment(socket_mesh_asset_);
}

void AAZSocketActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZSocketActor::SetSocketComponent(FName socket_name, USceneComponent* parent_component)
{
	if(parent_component == nullptr)//기존 부모 적용
	{
		if(socket_parent_actor_ != nullptr)//기존 부모가 있다면
		{
			this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			this->AttachToComponent(socket_parent_actor_, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socket_name);
		}
	}
	else//새로운 부모 적용
	{
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		this->AttachToComponent(parent_component, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socket_name);
		socket_parent_actor_ = parent_component;
	}
}
