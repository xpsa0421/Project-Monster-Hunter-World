// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <GameFramework/Actor.h>
#include "AZSocketActor.generated.h"

UCLASS()
class AZ_MHW_API AAZSocketActor : public AActor
{
	GENERATED_BODY()

public:
	AAZSocketActor();
 
protected:
	virtual void BeginPlay() override;

public:
	//현재 소켓 오브젝트(스켈레탈 메시)
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* socket_mesh_asset_;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* socket_fx_mesh_asset_;
	
	//소켓 부모 액터
	UPROPERTY(VisibleAnywhere)
	USceneComponent* socket_parent_actor_;
	
	//액터의 부착 소켓변경(생성자에서 호출불가. BeginPlay()부터 가능)
	void SetSocketComponent(FName socket_name, USceneComponent* parent_component = nullptr);

};
//auto SocketItem = GetWorld()->SpawnActor<AAZSocketActor>(FVector::ZeroVector,FRotator::ZeroRotator);
//if(SocketItem != nullptr) SocketItem->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);