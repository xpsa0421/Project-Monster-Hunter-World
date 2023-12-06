// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameMode/AZGameMode.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameStateMachine/AZStateMachine.h"
#include "AZ_MHW/GameStateMachine/AZGameState.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/Level/AZWorldSettings.h"
#include "AZ_MHW/PlayerController/AZPlayerController.h"

#include "Engine/AssetManager.h"
#include "Engine/LevelStreaming.h"
#include "Engine/WorldComposition.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "GameFramework/DefaultPawn.h"

AAZGameMode::AAZGameMode()
{
	audio_component_ = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audio_component_->SetTickableWhenPaused(true);
	audio_component_->bAutoActivate = false;
	RootComponent = audio_component_;

	DefaultPawnClass = ADefaultPawn::StaticClass();
	PlayerControllerClass = AAZPlayerController::StaticClass();
	HUDClass = AAZHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

	game_mode_flag_ = EGameModeFlag::None;

	is_started = false;
}


void AAZGameMode::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);
	game_instance_ = Cast<UAZGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if ()
}

void AAZGameMode::InitGameState()
{
	Super::InitGameState();
}

void AAZGameMode::StartPlay()
{
	//if (AZGameInstance->map_mgr == nullptr ||
	//	AZGameInstance->map_mgr->GetMapData() == nullptr)
	//{
		StartPlayManually();
	//}
}

void AAZGameMode::StartPlayManually()
{
	if (is_started == true)
	{
		return;
	}

	PreStartPlay();

	Super::StartPlay();

	USoundWave* sound_wave = GetBaseBGM();

	if (sound_wave != nullptr)
	{
		audio_component_->SetSound(sound_wave);

		audio_component_->Play();
	}

	if (state_machine != nullptr)
	{
		state_machine->OnStart();
	}

	PostStartPlay();

	is_started = true;
}

void AAZGameMode::OnGamePaused()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 0.0f);
}

void AAZGameMode::OnGamePauseReleased()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
}

TArray<ULevelStreaming*> GetAsyncLoadableStreamingLevel_Implement(TArray<ULevelStreaming*> all_streamings)
{
	TArray<ULevelStreaming*> result_array;
	/*if (AAZWorldSettings* world_setting = Cast<AAZWorldSettings>(game_instance_->GetWorld()->GetWorldSettings()))
	{
		int32 platform_map_count = world_setting->platform_level_name.Num();

		for (int32 index = 0; index < platform_map_count; ++index)
		{
			FString str_name = world_setting->platform_level_name[index];

			AAZGameMode* game_mode = AZGameInstance->GetGameMode();

			// FIXME need check
			// if (game_mode && game_mode->IsPcViewer())

			FName level_name = FName(*str_name);
			FLatentActionInfo latent_info;
			latent_info.UUID = index;

			for (int32 i = 0; i < all_streamings.Num(); ++i)
			{
				if (all_streamings[i]->GetWorldAssetPackageName().EndsWith(str_name))
				{
					result_array.Emplace(all_streamings[i]);
				}
			}
		}
	}*/
	return result_array;
}

TArray<ULevelStreaming*> AAZGameMode::GetAsyncLoadableStreamingLevel()
{
	return GetAsyncLoadableStreamingLevel_Implement(GetWorld()->GetStreamingLevels());
}

TArray<ULevelStreaming*> AAZGameMode::GetAsyncLoadableStreamingTile()
{
	return GetWorld()->WorldComposition ? GetAsyncLoadableStreamingLevel_Implement(GetWorld()->WorldComposition->TilesStreaming) : TArray<ULevelStreaming*>();
}

void AAZGameMode::LoadSyncExtraLevels()
{
	AZ_LOG("[AZAsyncLoad][AAZGameMode::LoadSyncExtraLevels] %d Extra Levels Exist", sync_load_level_name_list_.Num());
	for (int32 i = 0; i < sync_load_level_name_list_.Num(); ++i)
	{
		AZ_LOG("[AZAsyncLoad][AAZGameMode::LoadSyncExtraLevels] %s", *sync_load_level_name_list_[i].ToString());
		game_instance_->map_mgr->AddSubLevelFromPackageName(sync_load_level_name_list_[i]);
	}
	sync_load_level_name_list_.Empty();
	GetWorld()->FlushLevelStreaming(EFlushLevelStreamingType::Full);
}

void AAZGameMode::BeginAsyncLoad()
{
	if (IsValid(GetWorld()) == false)
	{
		return;
	}

	async_load_package_count_ = async_load_package_list_.Num();
	AZ_LOG("[AZAsyncLoad][AAZGameMode::BeginAsyncLoad] %d Packages Ready to AsyncLoad", async_load_package_count_);
	int32 cached_count = async_load_package_count_;
	if (async_load_package_count_ > 0)
	{
		for (int32 i = 0; i < cached_count; ++i)
		{
			switch (async_load_package_list_[i].load_type)
			{
				case EAZAsyncLoadType::Asset:
				{
					FName package_name = async_load_package_list_[i].package_name;
					async_load_package_list_[i].asset_handle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(FSoftObjectPath(package_name.ToString()),
						[this, package_name]()->void
						{
							for (int32 i = 0; i < async_load_package_list_.Num(); ++i)
							{
								if (async_load_package_list_[i].asset_handle.IsValid() && async_load_package_list_[i].package_name == package_name)
								{
									FSoftObjectPath loaded_asset_path(package_name.ToString());
									OnAsyncLoadCompleted(loaded_asset_path.GetAssetPathName(), nullptr,
										async_load_package_list_[i].asset_handle->HasLoadCompleted() ? EAsyncLoadingResult::Succeeded : async_load_package_list_[i].asset_handle->WasCanceled() ? EAsyncLoadingResult::Canceled : EAsyncLoadingResult::Failed);
								}
							}
						}, FStreamableManager::AsyncLoadHighPriority);
				}
				break;
				case EAZAsyncLoadType::Package:
				{
					AZ_LOG("[AZAsyncLoad][AAZGameMode::BeginAsyncLoad] (Package) %s", *async_load_package_list_[i].package_name.ToString());
					LoadPackageAsync(async_load_package_list_[i].package_name.ToString(), FLoadPackageAsyncDelegate::CreateUObject(this, &AAZGameMode::OnAsyncLoadCompleted));
				}
				break;
			}
		}
		game_mode_load_state_ = EAZGameModeLoadState::AsyncLoading;
	}
	else
	{
		game_mode_load_state_ = EAZGameModeLoadState::Loaded;
	}
}

void AAZGameMode::EndAsyncLoad()
{
	AZ_LOG("[AZAsyncLoad][AAZGameMode::EndAsyncLoad] Async Load Finished");

	game_mode_load_state_ = EAZGameModeLoadState::Loaded;

	if (GetWorld()->HasBegunPlay() == false)
	{
		game_instance_->map_mgr->StartPlayManuallyProc();
	}
}

void AAZGameMode::OnAsyncLoadCompleted(const FName& package_name, UPackage* loaded_package, EAsyncLoadingResult::Type result)
{
	FString result_str = result == EAsyncLoadingResult::Type::Succeeded ? TEXT("Succeeded") : result == EAsyncLoadingResult::Type::Failed ? TEXT("Failed") : TEXT("Canceled");
	AZ_LOG("[AZAsyncLoad][AAZGameMode::AsyncLoadPackage] Load Completed (%s) \"%s\"", *result_str, *package_name.ToString());

	if (async_load_package_list_.Num() <= 0)
	{
		AZ_LOG("[AZAsyncLoad][AAZGameMode::AsyncLoadPackage] Miss Complete Call (%s)", *package_name.ToString());
		return;
	}

	for (int32 i = 0; i < async_load_package_list_.Num(); ++i)
	{
		if (async_load_package_list_[i].package_name == package_name)
		{
			async_load_package_list_[i].OnLoadCompleted.ExecuteIfBound(package_name, loaded_package, result);
			--async_load_package_count_;

			float elapsed_time = GetWorld()->RealTimeSeconds - async_load_package_list_[i].load_start_time;
			AZ_LOG("[AZAsyncLoad][AAZGameMode::AsyncLoadPackage] Check in Package List (Elapsed %f sec, %d package left) \"%s\"", elapsed_time, async_load_package_count_, *async_load_package_list_[i].package_name.ToString());
			break;
		}
	}

	if (async_load_package_count_ <= 0)
	{
		async_load_package_list_.Empty();
		EndAsyncLoad();
	}
}

void AAZGameMode::GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToTransition, ActorList);

	// FixMe
	/*TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALHActor_LoadingScene::StaticClass(), actors);
	for (auto actor : actors)
	{
		ActorList.Add(actor);
	}*/
}

void AAZGameMode::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	//FixMe server, client switch gamemode(need map)
	//if (!bIsDisconnectSocketHandler)
	//	_RecvPacket();

	if (state_machine != nullptr)
	{
		state_machine->Tick(delta_seconds);
	}

	//if (GetWorld())
	//	PreRealTime = GetWorld()->GetRealTimeSeconds();
}

void AAZGameMode::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);

	if (state_machine != nullptr)
	{
		state_machine->ReleaseSubobjects();
	}
}

void AAZGameMode::OnGameMsg(FAZGameMsg* new_msg)
{
	if (state_machine != nullptr)
	{
		state_machine->OnGameMsg(new_msg);
	}
}

void AAZGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (game_instance_->GetPlayerController() != nullptr)
	{
		game_instance_->GetPlayerController()->SetShowMouseCursor(false);
	}
	//PlatformSetting();

	//PostLightLoaded();
}

USoundWave* AAZGameMode::GetBaseBGM()
{
	if (bgm_sound_wave_)
	{
		return bgm_sound_wave_;
	}

	// FIXME 맵 인덱스에 따라 bgm 틀어줌
	//if (ULHGameSingleton::Instance()->GetResourceMgr())
	//{
	//	return ULHGameSingleton::Instance()->GetResourceMgr()->GetBaseBGM(GetGameModeType(), LHGameInstance->MapMgr->GetMapIndex());
	//}

	return nullptr;
}

int32 AAZGameMode::CollectAsyncLoadPackage()
{
	if (game_mode_load_state_ == EAZGameModeLoadState::Loaded)
	{
		return 0;
	}
	else if (game_mode_load_state_ == EAZGameModeLoadState::AsyncLoading)
	{
		return 1;
	}

	game_mode_load_state_ = EAZGameModeLoadState::CollectingPackage;
	if (GetWorld()->WorldComposition != nullptr)
	{
		SetGamePaused(true);
	}
	BeginAsyncLoad();
	return int32();
}

UAZStateMachine* AAZGameMode::GetStateMachine() const
{
	return state_machine;
}

void AAZGameMode::SetGameState(EGameState state_id)
{
	check(state_machine);
	state_machine->ChangeState(state_id);
}

void AAZGameMode::SetPrevGameState()
{
	check(state_machine);
	state_machine->SetPreState();
}

UAZGameState* AAZGameMode::GetCurGameState() const
{
	if (state_machine == nullptr)
	{
		return nullptr;
	}

	return state_machine->GetCurState<UAZGameState>();
}

UAZGameState* AAZGameMode::GetGameState(EGameState state_id) const
{
	if (state_machine == nullptr)
	{
		return nullptr;
	}

	return state_machine->GetState(state_id);
}

EGameState AAZGameMode::GetCurGameStateID() const
{
	return GetStateMachine() ? GetStateMachine()->GetCurStateID() : EGameState::None;
}

void AAZGameMode::SetGamePaused(bool paused)
{
	if (game_paused_ == paused)
	{
		return;
	}

	game_paused_ = paused;
	if (game_paused_ == true)
	{
		OnGamePaused();
	}
	else
	{
		OnGamePauseReleased();
	}
}

bool AAZGameMode::GetGamePaused() const
{
	return game_paused_;
}
