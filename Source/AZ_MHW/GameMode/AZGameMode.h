// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AZ_MHW/ClientMsg/AZMsgObject.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/GameStateMachine/AZStateMachine.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "AZGameMode.generated.h"

UENUM()
enum class EAZAsyncLoadType
{
	Asset,
	Package
};

USTRUCT()
struct FAZAsyncLoadPackage
{
	GENERATED_BODY()

public:
	EAZAsyncLoadType load_type;
	FName package_name;
	TSharedPtr<struct FStreamableHandle> asset_handle;
	FLoadPackageAsyncDelegate OnLoadCompleted;
	float load_start_time;
};

UENUM()
enum class EAZGameModeLoadState
{
	Traveling,
	CollectingPackage,
	AsyncLoading,
	Loaded
};
UCLASS(abstract)
class AZ_MHW_API AAZGameMode : public AGameModeBase, public IAZMsgObject
{
	GENERATED_BODY()

public:
	AAZGameMode();

	virtual void InitGame(const FString& map_name, const FString& , FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual void StartPlay() override final;
	virtual void GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList) override;
	virtual void Tick(float delta_seconds) override;
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason) override;

	virtual void OnGameMsg(struct FAZGameMsg* new_msg) override;
	virtual void BeginPlay() override;

	virtual class USoundWave* GetBaseBGM();
	virtual void OnMapEnter() {}

public:
	virtual int32 CollectAsyncLoadPackage();

public:
	class UAZStateMachine* GetStateMachine() const;
	void SetGameState(EGameState state_id);
	void SetPrevGameState();
	class UAZGameState* GetCurGameState() const;
	UAZGameState* GetGameState(EGameState state_id) const;

	EGameState GetCurGameStateID() const;

	void SetGamePaused(bool paused);
	bool GetGamePaused() const;

public:
	virtual void StartPlayManually();

	virtual void PreStartPlay() {}
	virtual void PostStartPlay() {}
	virtual EGameModeType GetGameModeType() { return EGameModeType::None; }

protected:
	EGameModeFlag game_mode_flag_;

	uint8 game_paused_ : 1;

protected:
	virtual void OnGamePaused();
	virtual void OnGamePauseReleased();
	
	//비동기 로드
public:
	TArray<ULevelStreaming*> GetAsyncLoadableStreamingLevel();
	TArray<ULevelStreaming*> GetAsyncLoadableStreamingTile();
	virtual void LoadSyncExtraLevels();

protected:
	void BeginAsyncLoad();
	virtual void EndAsyncLoad();
	void OnAsyncLoadCompleted(const FName& package_name, class UPackage* loaded_package, EAsyncLoadingResult::Type result);

protected:
	UPROPERTY(EditAnywhere, Category = "AZ GameStateMachine")
	class TSubclassOf<class UAZStateMachine> state_machine_class;
	UPROPERTY() UAZStateMachine* state_machine;

private:
	bool is_started = false;

protected:
	UPROPERTY() TArray<FAZAsyncLoadPackage> async_load_package_list_;
	UPROPERTY() TArray<FName> sync_load_level_name_list_;
	EAZGameModeLoadState game_mode_load_state_;
	int32 async_load_package_count_;

protected:
	UPROPERTY() class UAudioComponent* audio_component_;

	UPROPERTY()
	class USoundWave* bgm_sound_wave_;

	UPROPERTY() class UAZGameInstance* game_instance_;
};
