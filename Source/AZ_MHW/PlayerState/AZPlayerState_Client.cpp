// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"

AAZPlayerState_Client::AAZPlayerState_Client()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void AAZPlayerState_Client::BeginPlay()
{
	OnPawnSet.AddDynamic(this, &AAZPlayerState_Client::SetPlayerCharacter);
	Super::BeginPlay();
}

void AAZPlayerState_Client::BeginDestroy()
{
	OnPawnSet.RemoveDynamic(this,&AAZPlayerState_Client::SetPlayerCharacter);
	Super::BeginDestroy();
}

void AAZPlayerState_Client::SetPlayerCharacter(APlayerState* player_state, APawn* new_pawn, APawn* old_pawn)
{
	if(const auto player_character = Cast<AAZPlayer>(old_pawn))
	{
		player_character->player_character_state_ = nullptr;
	}
	
	if(const auto player_character = Cast<AAZPlayer>(new_pawn))
	{
		player_character->player_character_state_ = this;

		//PlayerState의 형태
		player_character->SetSKMeshParts();//PlayerState의 파츠메시
		player_character->SetHairColor();//PlayerState의 색상컬러
		player_character->SetSKMeshSocket();//PlayerState의 소켓메시
		player_character->SetSKMeshEfxMaterial();//PlayerState의 이펙트 메시 머테리얼 설정
		player_character->SetEnableSKMeshEfx(false);//PlayerState의 이펙트 머테리얼 끄기
	}
}
