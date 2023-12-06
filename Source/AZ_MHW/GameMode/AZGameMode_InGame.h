// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "functional"
#include "Engine/EngineTypes.h"
#include "AZGameMode_InGame.generated.h"

class UAZObjectMgr_Client;
class ULevelStreamingDynamic;

UCLASS()
class AZ_MHW_API AAZGameMode_InGame : public AAZGameMode
{
	GENERATED_BODY()
	
public:
	AAZGameMode_InGame();

	virtual void Tick(float delta_seconds) override;
	
	virtual void InitGame(const FString& map_name, const FString& options, FString& error_message) override;
	virtual void PreStartPlay() override;
	virtual void PostStartPlay() override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason) override;

	virtual void OnGameMsg(struct FAZGameMsg* msg) override;

	virtual void RestartPlayerAtPlayerStart(AController* new_player, AActor* start_spot) override;

	virtual void PostLogin(APlayerController* new_player) override;
	
	virtual void OnMapEnter() override;
	
	void OnMapExit();

	virtual EGameModeType GetGameModeType() { return EGameModeType::InGame; }

public:
	class UAZWorldMap* GetWorldMapHandler();

	void OnCharacterDisappeared(class AAZCharacter* character, int32 object_serial = 0);
	void OnActorDisappeared(class AActor* actor, int32 object_serial = 0);


	bool RemoveAppearCharacter(int32 object_serial);
	bool RemoveAppearActor(int32 object_serial);
	void RemoveAllAppearCharacter();
	void RemoveAllAppearObjects();

	class AActor* FindAppearCharacterAndActors(int32 object_serial, bool include_playable_palyer = true);
	class AAZCharacter* FindAppearCharacter(int32 object_serial, bool include_playable_player = true);
	TArray<class AAZCharacter*> FindAppearCharactersFromSpawnIndex(int32 spawn_index);
	class AAZPlayer* FindAppearPlayer(int32 object_serial, bool include_playable_player = true);
	class AAZPlayer_Remote* FindAppearRemotePlayer(int64 player_db_key);
	class AAZCharacter* FindAppearNpcCharacter(int32 data_key);
	AActor* FindAppearActors(int32 object_serial);
	typedef std::function<void(class AAZCharacter*)> OnForEachCharacterDelegate;
	void ForEachAppearCharacters(OnForEachCharacterDelegate func, bool include_playable_player = true);
	typedef std::function<void(class AAZNpc*)> OnForEachNpcDelegate;
	void ForEachAppearNpc(OnForEachNpcDelegate func);
	typedef std::function<void(class AAZActor_Interaction*)> OnForEachInteractionDelegate;
	void ForEachAppearInteraction(OnForEachInteractionDelegate func);
	void ForEachBossMonsters(OnForEachCharacterDelegate func);

	// TEMP
	UFUNCTION(BlueprintCallable) void OpenQuestWidget(); 
	UFUNCTION() void RequestWarpCombatLevel();
	void OnCombatLevelLoaded(ULevel* in_level, UWorld* in_world);

public:
	DECLARE_EVENT_OneParam(AAZGameMode_InGame, FAppearObject, AActor*);
	FAppearObject OnAppearObjectEvent;
	DECLARE_EVENT_OneParam(AAZGameMode_InGame, FDisappearObject, int32);
	FDisappearObject OnDisappearObjectEvent;

	UPROPERTY() TObjectPtr<UAZObjectMgr_Client> object_mgr_;
	TObjectPtr<ULevelStreamingDynamic> combat_level_;
	
protected:
	UPROPERTY() TMap<int32, class AActor*> appear_actors_;
	// FIXME merged check	
	//UPROPERTY() TMap<int32, class AAZCharacter*> appear_character_;
	//UPROPERTY() TMap<int32, class AAZCharacter*> appear_npc_;
	//UPROPERTY() TArray<class AAZCharacter*> boss_monsters_;

private:
	UPROPERTY() class UAZWorldMap* world_map_handler_;
	
};
