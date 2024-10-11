// Copyright Team AZ. All Rights Reserved.


#include "GameMode/AZGameMode_InGame.h"

#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Map/AZWorldMap.h"
#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/Environment/interaction/AZActor_Interaction.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/Manager/AZObjectMgr_Client.h"
#include "AZ_MHW/PlayerController/AZPlayerController_InGame.h"
#include "Engine/LevelStreamingDynamic.h"
#include "HUD/AZHUD.h"
#include "HUD/AZHUDDefine.h"
#include "SocketHolder/AZSocketHolder.h"
#include "Widget/MsgBox/AZWidget_MsgBoxBasic.h"

AAZGameMode_InGame::AAZGameMode_InGame()
{
	game_mode_flag_ = EGameModeFlag::InGame;
	PlayerControllerClass = AAZPlayerController_InGame::StaticClass();
	PlayerStateClass = AAZPlayerState_Client::StaticClass();
	DefaultPawnClass = nullptr;
}

void AAZGameMode_InGame::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZGameMode_InGame::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);
}

void AAZGameMode_InGame::PreStartPlay()
{
	Super::PreStartPlay();
	object_mgr_ = NewObject<UAZObjectMgr_Client>(this);
}

void AAZGameMode_InGame::PostStartPlay()
{
	Super::PostStartPlay();
}

void AAZGameMode_InGame::BeginPlay()
{
	Super::BeginPlay();
}

void AAZGameMode_InGame::Destroyed()
{
	Super::Destroyed();
}

void AAZGameMode_InGame::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);
}

void AAZGameMode_InGame::OnGameMsg(FAZGameMsg* msg)
{
	Super::OnGameMsg(msg);
}

void AAZGameMode_InGame::RestartPlayerAtPlayerStart(AController* new_player, AActor* start_spot)
{
	Super::RestartPlayerAtPlayerStart(new_player, start_spot);
}

void AAZGameMode_InGame::PostLogin(APlayerController* new_player)
{
	//Server
	Super::PostLogin(new_player);
}

void AAZGameMode_InGame::OnMapEnter()
{
	Super::OnMapEnter();
}

void AAZGameMode_InGame::OnMapExit()
{
	if (world_map_handler_ != nullptr)
	{
		world_map_handler_->ClearWorldMapInfo();
	}
}

UAZWorldMap* AAZGameMode_InGame::GetWorldMapHandler()
{
	return world_map_handler_;
}

void AAZGameMode_InGame::OnCharacterDisappeared(AAZCharacter* character, int32 object_serial)
{
	// FIXME merged need alive
	/*if (IsValid(character) == false)
	{
		return;
	}

	if (object_serial <= 0)
	{
		object_serial = character->GetObjectSerial();
	}

	if (appear_character_.Contains(object_serial) == false)
	{
		return;
	}

	if (AAZPlayer_Playable* player = GetPlayablePlayer())
	{
		player->NoticeDisappear(character);
	}

	if (appear_npc_.Contains(character->GetObjectDataKey()))
	{
		appear_npc_.Remove(character->GetObjectDataKey());
	}

	if (boss_monsters_.Contains(character))
	{
		boss_monsters_.Remove(character);
	}

	if (AAZMonster* monster = Cast<AAZMonster>(character))
	{
		if (monster->GetCharacterState() != ECharacterState::Die && monster->GetCharacterState() != ECharacterState::Die_End &&
			monster->IsReceiveDie() && AZGameInstance->map_mgr->isInDungeon())
		{
			AZGameInstance->map_mgr->OnDefeatMonsterInDungen(monster);
		}
	}

	character->DisappearProc();
	character->Destroy();

	if (OnDisappearObjectEvent.IsBound())
	{
		OnDisappearObjectEvent.Broadcast(object_serial);
	}*/
}

void AAZGameMode_InGame::OnActorDisappeared(AActor* actor, int32 object_serial)
{
	//FIXME after Appear obejct create comment released
	/*if (IsValid(actor) == false)
		return;

	if (object_serial <= 0)
	{
		for (auto& kv : appear_actors_)
		{
			if (kv.Value == actor)
			{
				object_serial = kv.Key;
				break;
			}
		}
	}

	if (appear_actors_.Contains(object_serial) == false)
	{
		return;
	}

	if (AAZPlayer_Playable* player = GetPlayablePlayer())
	{
		player->NoticeDisappear(actor);
	}

	if (IAZActor_Interface* remove_interface = Cast<IAZActor_Interface>(actor))
	{
		remove_interface->DisappearProc();
	}

	actor->Destroy();

	if (OnDisappearObjectEvent.IsBound())
	{
		OnDisappearObjectEvent.Broadcast(object_serial);
	}*/
}

bool AAZGameMode_InGame::RemoveAppearCharacter(int32 object_serial)
{
	if (AAZCharacter* remove_character = FindAppearCharacter(object_serial))
	{
		OnCharacterDisappeared(remove_character, object_serial);
		// FIXME merged release
		//appear_character_.Remove(object_serial);
		return true;
	}
	return false;
}

bool AAZGameMode_InGame::RemoveAppearActor(int32 object_serial)
{
	if (AActor* remove_actor = FindAppearActors(object_serial))
	{
		OnActorDisappeared(remove_actor, object_serial);
		// FIXME merged release
		//appear_actors_.Remove(object_serial);
		return true;
	}
	return false;
}

void AAZGameMode_InGame::RemoveAllAppearCharacter()
{
	// FIXME merged release
	/*for (auto& appear_character : appear_character_)
	{
		if (appear_character.Value)
		{
			OnCharacterDisappeared(appear_character.Value);
		}
	}

	appear_character_.Empty();
	appear_npc_.Empty();*/
}

void AAZGameMode_InGame::RemoveAllAppearObjects()
{
	// FIXME merged release
	/*for (auto& appear_character : appear_character_)
	{
		if (appear_character.Value)
		{
			OnCharacterDisappeared(appear_character.Value);
		}
	}*/

	for (auto& appear_actor : appear_actors_)
	{
		if (appear_actor.Value)
		{
			OnActorDisappeared(appear_actor.Value);
		}
	}

	//FIXME merged release
	/*appear_character_.Empty();
	appear_npc_.Empty();*/
	appear_actors_.Empty();
}

AActor* AAZGameMode_InGame::FindAppearCharacterAndActors(int32 object_serial, bool include_playable_player)
{

	AActor* found_actor = nullptr;
	// FiXME merged release
	//found_actor = FindAppearCharacter(object_serial, include_playable_player);
	if (found_actor == nullptr)
	{
		found_actor = FindAppearActors(object_serial);
	}

	return found_actor;
}

AAZCharacter* AAZGameMode_InGame::FindAppearCharacter(int32 object_serial, bool include_playable_player)
{
	if (include_playable_player == true)
	{
		// FIXME merged release
		/*if (GetPlayablePlayer() && GetPlayablePlayer()->GetObjectSerial() == object_serial)
		{
			return GetPlayablePlayer();
		}*/
	}

	//FIXME merged release
	/*if (auto found_character = appear_character_.Find(object_serial))
	{
		return *found_character;
	}*/

	return nullptr;
}

TArray<class AAZCharacter*> AAZGameMode_InGame::FindAppearCharactersFromSpawnIndex(int32 spawn_index)
{
	TArray<class AAZCharacter*> characters;
	//FIXME merged release
	/*for (auto& kv : appear_character_)
	{
		auto* character = kv.Value;
		if (IsValid(character) == false)
		{
			continue;
		}

		if (character->GetSpawnIndex() == spawn_index)
		{
			character.Emplace(character);
		}
	}*/
	return characters;
}

AAZPlayer* AAZGameMode_InGame::FindAppearPlayer(int32 object_serial, bool include_playable_player)
{
	if (include_playable_player == true)
	{
		// FIXME merged release
		/*if (GetPlayablePlayer() && GetPlayablePlayer()->GetObjectSerial() == object_serial)
		{
			return GetPlayablePlayer();
		}*/
	}

	//FIXME merged release
	/*if (auto found_character = appear_character_.Find(object_serial))
	{
		return Cast<AAZPlayer>(*found_character);
	}*/

	return nullptr;
}

AAZPlayer_Remote* AAZGameMode_InGame::FindAppearRemotePlayer(int64 player_db_key)
{
	// FIXME merged release
	/*for (auto& character : appear_character_)
	{
		AAZPlayer_Remote* player = Cast<AAZPlayer_Remote>(character.Value);

		if (player && player->player_db_key)
		{
			return player;
		}
	}*/
	return nullptr;
}

AAZCharacter* AAZGameMode_InGame::FindAppearNpcCharacter(int32 data_key)
{
	// FIXME merged release
	/*auto found_character = appear_npc_.Find(data_key);
	return found_character ? *found_character : nullptr;*/
	return nullptr;
}

AActor* AAZGameMode_InGame::FindAppearActors(int32 object_serial)
{
	auto found_actor = appear_actors_.Find(object_serial);
	return found_actor ? *found_actor : nullptr;
}

void AAZGameMode_InGame::ForEachAppearCharacters(OnForEachCharacterDelegate func, bool include_playable_player)
{
	/*if (include_playable_player == true)
	{
		auto player = GetPlayablePlayer();
		if (IsValid(player))
		{
			func(player);
		}
	}

	for (auto& info : appear_character_)
	{
		if (IsValid(info.Value))
		{
			func(info.Value);
		}
	}*/
}

void AAZGameMode_InGame::ForEachAppearNpc(OnForEachNpcDelegate func)
{
	// FIXME need NPC create -> release comment
	/*for (auto& info : appear_npc_)
	{
		func(Cast<AAZNpc>(info.Value));
	}*/
}

void AAZGameMode_InGame::ForEachAppearInteraction(OnForEachInteractionDelegate func)
{
	for (auto& info : appear_actors_)
	{
		if (AAZActor_Interaction* interaction_obj = Cast<AAZActor_Interaction>(info.Value))
		{
			func(interaction_obj);
		}
	}
}

void AAZGameMode_InGame::ForEachBossMonsters(OnForEachCharacterDelegate func)
{
	// FIXME merged release
	/*for (AAZCharacter* boss : boss_monsters_)
	{
		if (!IsValid(boss))
		{
			continue;
		}
		func(boss);
	}*/
}

//----------------------------------------------------------------------------
// TEMP TEST
// TODO 캐릭터/위젯 로직으로 옮기기. 맵이동 TEST용
void AAZGameMode_InGame::OpenQuestWidget()
{
	// playercontroller 없어서 테스트 불가
	auto msg_box = game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("QuestTemp"), EUIMsgBoxBtnType::Confirm,
	this, TEXT(""), L"", L"", L"확인");

	if (msg_box)
	{
		UAZWidget_MsgBoxBasic* msg_box_basic = Cast<UAZWidget_MsgBoxBasic>(msg_box);
		if (msg_box_basic)
		{
			msg_box_basic->SetTitle("전투맵 이동 테스트");
			msg_box_basic->AddHandler(EMsgEventButtonType::Left, this, FName(TEXT("RequestWarpCombatLevel")));
		}
	}
}

void AAZGameMode_InGame::RequestWarpCombatLevel()
{
	game_instance_->BindLevelAddRemoveEvents();
	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &AAZGameMode_InGame::OnCombatLevelLoaded);
	
	// TEMP JOIN PACKET
	CS_COMBAT_MAP_ENTER_REQ packet;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
	
	bool is_succeeded = false;
	// 패키징할때 설명 확인 MUST
	combat_level_ = ULevelStreamingDynamic::LoadLevelInstance(game_instance_, TEXT("Map_MainStreaming"),
		FVector::ZeroVector, FRotator::ZeroRotator, is_succeeded);
	combat_level_->SetShouldBeLoaded(true);
	combat_level_->SetShouldBeVisible(true);
}

void AAZGameMode_InGame::OnCombatLevelLoaded(ULevel* in_level, UWorld* in_world)
{
	FWorldDelegates::LevelAddedToWorld.RemoveAll(this);
	
	// TEMP LOAD FINISHED PACKET
	CS_COMBAT_MAP_LOAD_FINISH_CMD packet;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);

	OnEnterCombatLevel.Broadcast();
}
