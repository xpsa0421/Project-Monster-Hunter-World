// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZMapMgr.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Map/AZMapBase.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/Manager/AZResourceMgr.h"
#include "AZ_MHW/GameMode/AZGameMode_InGame.h"
#include "AZ_MHW/Map/AZWorldMap.h"
#include "AZ_MHW/Widget/InGame/AZWidget_InGame.h"
#include "AZ_MHW/Widget/Login/AZWidget_Loading.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/Map/AZMapBase.h"
#include "AZ_MHW/CommonSource/AZGameMsg.h"
#include "AZ_MHW/Environment/AZActor_LoadingScene.h"
#include "AZ_MHW/Widget/Login/AZWidget_Loading.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/CommonSource/Table/ModelData.h"
#include "AZ_MHW/CommonSource/Table/LoadingData.h"
#include "AZ_MHW/CommonSource/Table/MapData.h"
#include "AZ_MHW/Environment/AZActor_LoadingScene.h"
#include "Components/SkyLightComponent.h"
#include "Components/LightComponent.h"
#include "ShaderPipelineCache.h"
#include "Math/Box.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBaseUtility.h"
#include "Engine/WorldComposition.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/Character.h"

const FName TAG_LoadingCheck(TEXT("AZ_LoadingCheck"));

UAZMapMgr::UAZMapMgr()
{
	loop_timer_ = 0.25;
	is_map_entered_ = false;
	is_portal_moving_ = false;
	enter_pos_ = FVector::ZeroVector;
	is_loading_ = false;
	close_loading_scene_on_finish_ = false;
	flush_streaming_ = false;
	ready_server_sync_streaming_ = false;
	use_teleport_after_sync_streaming_ = false;
	current_streaming_state_ = EStreamingState::None;
	reserved_force_kick_ = EForceKick::Max;
	same_map_enter_ready_ = false;
	saved_auto_move_data_.Reset();
	loading_map_index_ = 0;
	disconnected_caching_map_index_ = 0;

}

UAZMapMgr::~UAZMapMgr()
{
	// FIXME 아레아 맵 초기화
}

void UAZMapMgr::Init()
{
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	same_map_enter_ready_ = false;
}

void UAZMapMgr::LobbyEnterAck()
{
	MapDestroy();
}

void UAZMapMgr::Tick(float delta_time)
{
	if (GetDefault<UAZMapMgr>(UAZMapMgr::StaticClass()) == this)
	{
		return;
	}

	UpdateStreaming();

}

void UAZMapMgr::OnGameMsg(FAZGameMsg* new_msg)
{
	if (IsValid(current_map_))
	{
		current_map_->OnGameMsg(new_msg);
	}
}

bool UAZMapMgr::IsTickable() const
{
	return !HasAnyFlags(EObjectFlags::RF_ClassDefaultObject);
}

bool UAZMapMgr::IsTickableInEditor() const
{
	return false;
}

bool UAZMapMgr::IsTickableWhenPaused() const
{
	return false;
}

void UAZMapMgr::MapEnter(uint32 map_index, FVector enter_pos, float enter_angle)
{
	if (reserved_force_kick_ != EForceKick::Max)
	{
		game_instance_->login_mgr->OnForceKicked(reserved_force_kick_);
		reserved_force_kick_ = EForceKick::Max;
		return;
	}


	// 이전 맵에서 나감
	OnMapExit();

	enter_pos_ = enter_pos;
	enter_angle_ = enter_angle;

	const UMapData* before_map_data = GetMapData();
	if (current_map_)
	{
		current_map_->OnMapExit();
	}

	current_map_ = CreateMap(map_index);
	current_map_->OnCreate(GetTableByIndex(UMapData, map_index));


	dungeon_info_ = nullptr;
	if (Cast<UAZDungeon>(current_map_))
	{
		dungeon_info_ = Cast<UAZDungeon>(current_map_);
	}

	const UMapData* map_data = GetMapData();

	// FIXME 스포너 설치하기(나중에 넣기)
	//{

	//}

	bool is_open_map = true;
	if (before_map_data && map_data->map_name.Compare(before_map_data->map_name) == 0)
	{
		is_open_map = false;
	}

	// 맵을 로딩하고 Player를 위치시킴
	if (map_data != nullptr)
	{
		if (is_open_map == true)
		{
			AZResourceHelper::ClearReference(EAZResourceCacheType::MapOnly);

			if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode()))
			{
				game_mode->GetWorldMapHandler()->ClearWorldMapInfo();
			}

			FString map_path = FString::Printf(TEXT("/Game/Maps/%s"), *map_data->map_name);

			OpenLoadingScene(map_index);

			GEngine->ForceGarbageCollection(true);

			bool use_seamless_loading = true;
			if (use_seamless_loading == true)
			{
				game_instance_->GetWorld()->SeamlessTravel(map_path);
			}
			else
			{
				FName map_name(*map_path);
				UGameplayStatics::OpenLevel(game_instance_->GetWorld(), map_name);
			}

			post_load_map_delegate_handle_ = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAZMapMgr::PostLoadMapFunction);

			/*if (AZGameInstance != nullptr)
			{
				AZGameInstance->voidSummonerMgr->ResetSpawnnedInfo();
			}

			if (AZGameInstance->SoundAlarmMgr)
			{
				AZGameInstance->SoundAlarmMgr->ClearAllTimers();
			}

			if (AZGameInstance->InputMgr)
			{
				AZGameInstance->InputMgr->ClearBlockFlag();
			}*/
		}

		/*if (AZGameInstance && AZGameInstance->IsRealmWar())
		{
			if ((EMapType)map_data->map_type != EMapType::RealmWar)
			{
				AZGameInstance->RealmWarMgr->EndRealmwar();
			}
		}*/

		if (is_open_map == false) // 들어오지는 않을거 같음
		{
			bool immediately_same_map_enter_proc = true;
			// 원래 로비맵으로 돌아옴
			EMapType map_type = UAZUtility::StringToEnum<EMapType>(map_data->map_type);
			if (EMapType::Lobby == static_cast<EMapType>(map_type))
			{
				//if (LHGameInstance->QuestMgr->IsReservedBeginChapter())
				//{
				//	if (ALHGameMode_InGame* gm = Cast<ALHGameMode_InGame>(LHGameInstance->GetGameMode()))
				//	{
				//		if (AAZPlayer_Playable* player = AZGameInstance->GetPlayer())
				//		{
				//			if (player->GetNpcTalkComponent()->IsTalk())
				//				player->EndNpcTalk();
				//		}
				//		bImmediatelySameMapEnterProc = false;
				//		gm->GetStateMachine()->ChangeState(EGameState::BeginChapter);

				//		auto func = [this, enterPos, enterAngle]()->void			// 지금 말고 나중에 연출 끝나면 위치 이동 
				//		{
				//			this->SameMapEnterProc(enterPos, enterAngle);
				//		};
				//		CallBackQuestWarp = func;
				//	}
				//}
				//else
				//{
				//	if (ULHWidget_BlackInOut* ui = LHGameInstance->GetHUD()->OpenUI<ULHWidget_BlackInOut>(EUIName::LHWidget_BlackInOut))
				//	{
				//		auto func = [ui, this, enterPos, enterAngle]()->void
				//		{
				//			ui->PlayExit();
				//			this->SameMapEnterProc(enterPos, enterAngle);
				//		};
				//		bImmediatelySameMapEnterProc = false;
				//		ui->SetCallBackFunc(func);
				//	}
				//}

				//if (UAZWidget_InGame* ui = LHGameInstance->GetHUD()->GetUI<ULHWidget_InGame>(EUIName::LHWidget_InGame))
				//{
				//	ui->ChangeExitSwitcher(false);
				//}

				//if (ALHGameMode_InGame* gm = Cast<ALHGameMode_InGame>(LHGameInstance->GetGameMode()))
				//{
				//	gm->ActiveEnvironmentReset();
				//}
			}
			else if (EMapType::Dungeon == map_type)
			{
				/*if (UAZWidget_InGame* ui = AZGameInstance->GetHUD()->GetUI<UAZWidget_InGame>(EUIName::AZWidget_InGame))
				{
					ui->ChangeExitSwitcher(true);
				}*/
			}

			if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode()))
			{
				//game_mode->ResetAreaPortalAndWarp();
				game_mode->RemoveAllAppearObjects();
			}
		}
	}
}

void UAZMapMgr::SameMapEnterPorc(FVector enter_pos, float enter_angle)
{
	same_map_enter_ready_ = true;

	// FIXME 병합시 풀기
	/*AAZPlayer_Playable* player = AZGameInstance->GetPlayer();
	if (player != nullptr)
	{
		// 물리적으로 같은 맵이라면 위치 이동
		player->OnWarp(enter_pos, enter_angle);

		// FIXME 이동 시 탈 것 내리기 (서버에서는 내리는 것으로 처리)
	}*/

	// FIXME 메인메뉴 딤드 갱신
	
	// FIXME 퀘스트 ui 없애기

	CloseLoadingScene();
	SendMapEnter();
}

void UAZMapMgr::PostLoadMapFunction(UWorld* world)
{
	AZ_PRINT_LOG_IF_FALSE(world);

	pause_streaming_ = false;
	character_only_streaming_ = false;
	current_streaming_state_ = world->WorldComposition ? Waiting : None;

	TArray<AActor*> actors;
	// FIXME 병합시 풀기
	/*UGameplayStatics::GetAllActorsOfClass(world, AAZPlayer_Playable::StaticClass(), actors);
	if (actors.Num() > 0)
	{
		if (AAZPlayer_Playable* start = Cast<AAZPlayer_Playable>(actors[0]))
		{
			start->SetActorServerLocation(GetEnterPos());
			if (auto* movement = start->GetAZCharacterMovement_Sync())
			{
				movement->SetIgnoreSync(true);
			}
		}
	}*/

	if (world->WorldComposition)
	{
		//FIXME 플러그인에서 재정의 한것으로 보임
		/*if (world->bDisableUpdateStreamingState == false)
		{
			world->bDisableUpdateStreamingState = true;
		}*/

		if (world->WorldComposition->GetOutermost()->HasAnyPackageFlags(PKG_PlayInEditor))
		{
			world->WorldComposition->ReinitializeForPIE();
		}

		RenewLevelStreamingState();
	}
	else
	{
		StartPlayManuallyProc();
	}

	game_instance_->InitSocketOnMapLoad();

	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(post_load_map_delegate_handle_);
}

void UAZMapMgr::MapChannelMove(int32 map_rec_key, int32 channel_index)
{
	if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode()))
	{
		game_mode->RemoveAllAppearObjects();
	}

	SendMapEnter();
	UE_LOG(LogClass, Log, TEXT("[CHANNEL MOVE] map(%d), channel(%d)"), map_rec_key, channel_index);
}

void UAZMapMgr::OnMapEnter_Ack()
{
	is_map_entered_ = true;

	if (IsValid(current_map_))
	{
		current_map_->OnStart();
	}

	if (same_map_enter_ready_ && game_instance_->GetPlayer())
	{
		same_map_enter_ready_ = false;
		// FIXME 병합시 확인하기
		//AZGameInstance->GetPlayer()->ReleaseMapMove();
	}

	if (UAZWidget_InGame* ui = game_instance_->GetHUD()->GetUI<UAZWidget_InGame>(EUIName::AZWidget_InGame))
	{
		// FIXME 퀘스트 추가시 UI변경하기
		/*if (AZGameInstance->QuestMgr->GetStartQuestInformIndex() > 0 && GetCurrentMapType() != EMapType::RealmWar)
		{
			ui->SetLeftTabVisible(false);
			AZGameInstance->QuestMgr->TotalUpdateQuestUI();
		}
		else
		{
			AZGameInstance->QuestMgr->TotalUpdateQuestUI();
		}*/
	}

	// 
	/*if (IsAutoMovePortalReserved())
	{
		// FIXME 순간이동 종류

		ResetAutoMovePortalInfo();
	}
	// FIXME 자동이동은 나중에(풀벌레에 쓰면 좋을듯
	else if (save_auto_move_data.MapIdx != -1)
	{
		SetAutoMove(saved_auto_move_data.map_index, saved_auto_move_data.auto_move_type, saved_auto_move_data.target_location);
	}
	*/
}

void UAZMapMgr::OnMapExit()
{
	AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(UGameplayStatics::GetGameMode(game_instance_->GetWorld()));
	if (game_mode)
	{
		game_mode->OnMapExit();
	}

	// FIXME Area 정보 초기화
	//MapArea.Empty();

	// FIXME 포탈정보 초기화
	//PortalMoveTriggerList.Empty();

	is_map_entered_ = false;

	// FIXME Character에게 맵을 나갔다 알림
	// AZGameInstance->Characdter_Mgr->OnMapExit();
}

bool UAZMapMgr::OnRequestMapOut()
{
	if (current_map_ == nullptr)
	{
		return false;
	}

	return current_map_->OnRequestMapOut();
}

void UAZMapMgr::SendMapEnter()
{
	if (IsMapEnterRequestReserved())
	{
		// FIXME 맵 나감 메시지
		//FCG_MAP_ENTER_REQUEST_CMD msg;
		//msg.map_index = reserved_map_enter_request_.map_index;
		ResetMapEnterRequest();
		// FIXME 서버들어 갈시 확인하기
		//OnRecvMapEnterRequest(msg, true);
		return;
	}

	if (current_map_ == nullptr)
	{
		return;
	}

	// FIXME 합병시 확인하기
	/*UAZSocketHolder* game_socket_holder = AZGameInstance->GetSocketHolder(ESocketHolderType::Gate);
	if (game_socket_holder != nullptr)
	{
		FCG_MAP_ENTER_SYN msg_syn;
		msg_syn.map_index = GetMapIndex();
		game_socket_holder->SendPacket(&msg_syn);
	}*/

	// FIXME 합병시 확인하기
	/*
	if (AZGameInstance && AZGameInstance->GetPlayer())
	{
		AZGameInstance->GetPlayer()->OnMapEnter();
	}
	*/

	game_instance_->GetGameMode()->OnMapEnter();
}

void UAZMapMgr::StartPlayManuallyProc()
{
	if (game_instance_->GetGameMode()->CollectAsyncLoadPackage() <= 0)
	{
		// FIXME 솔로 던전에서 확인하기
		/*if (GetCurrentMapType() == EMapType::AloneQUestDungeon)
		{
			PreLoadResources_Sync();
		}*/

		if (game_instance_->GetWorld()->WorldComposition)
		{
			game_instance_->GetWorld()->PopulateStreamingLevelsToConsider();
			FlushStreaming();
		}
		else
		{
			game_instance_->GetGameMode()->SetGamePaused(false);
			AZ_LOG("No WorldComposition in AAZGameMode_InGame");
		}

		game_instance_->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::FirstTickAfterStartPlayManuallyProc));
	}
}

void UAZMapMgr::MapDestroy()
{
	if (dungeon_info_ == current_map_)
	{
		dungeon_info_ = nullptr;
	}

	if (current_map_)
	{
		current_map_->OnMapExit();
		current_map_->ConditionalBeginDestroy();
	}
	current_map_ = nullptr;
}

void UAZMapMgr::MapExit_Syn()
{
	// FIXME 병합 시 확인하기
	/*if (UAZSocketHolder* game_socket_holder = AZGameInstance->GetSocketHolder(ESocketHolderType::Game))
	{
		FCG_MAP_EXIT_SYN msg_syn;
		msg_syn.is_escape = false;
		game_socket_holder->SendPacket(&msg_syn);
	}*/
}

void UAZMapMgr::FirstTickAfterStartPlayManuallyProc()
{
	game_instance_->GetGameMode()->StartPlayManually();
	RenewLevelStreamingState(false, game_instance_->GetPlayer(), true);
	game_instance_->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::SecondTickAfterStartPlayManuallyProc));
}

void UAZMapMgr::SecondTickAfterStartPlayManuallyProc()
{
	if (!game_instance_->GetWorld()->WorldComposition)
	{
		StopCapture();
		game_instance_->GetHUD()->OnFadeInOut(0.5f, 0.5f);
		FTimerHandle handle;
		game_instance_->GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateUObject(this, &UAZMapMgr::ThirdTickAfterStartPlayManuallyProc), 0.5f, false);
		SendMapEnter();
	}

	if (IsInDungeon())
	{
		OnBeginDungeon();
	}
}

void UAZMapMgr::ThirdTickAfterStartPlayManuallyProc()
{
	CloseLoadingScene();
	game_instance_->GetGameMode()->LoadSyncExtraLevels();

	if (reserved_force_kick_ != EForceKick::Max)
	{
		game_instance_->login_mgr->OnForceKicked(reserved_force_kick_);
		reserved_force_kick_ = EForceKick::Max;
	}
}

void UAZMapMgr::ProcessDisappearObject(int32 object_serial)
{
	if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode()))
	{
		auto* object = game_mode->FindAppearActors(object_serial);

		bool success = game_mode->RemoveAppearCharacter(object_serial);
		if (success == true)
		{
			UE_LOG(LogClass, Log, TEXT("[FCG_MAP_DISAPPEAR_OBJECT_CMD][M:%d] Disappear Character"), object_serial)
		}
		else
		{
			success = game_mode->RemoveAppearActor(object_serial);
			if (success == true)
			{
				UE_LOG(LogClass, Log, TEXT("disappear_object[m:%d] disappear actor"), object_serial);
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("disappear_object[m:%d] not found actor"), object_serial);
			}
		}

		FDestroyedObject msg(object_serial);
		msg.object_ptr = object;
		game_instance_->OnGameMsg(&msg);

		// FIXME 병합시 추가하기
		/*if (AAZPlayer_Playable* player = AZGameInstance->GetPlayer())
		{
			player->TargetFinder->OnTargetDisappeared(object_serial);
		}*/
	}
}

bool UAZMapMgr::IsInMapType(EMapType type) const
{
	if (IsInMap() == false)
	{
		return false;
	}
	EMapType map_type = UAZUtility::StringToEnum<EMapType>(GetMapData()->map_type);
	if (map_type != type)
	{
		return false;
	}
	return true;
}

int32 UAZMapMgr::GetMapIndex() const
{
	const auto map_data = GetMapData();
	return map_data ? map_data->id : 0;
}

UAZMapBase* UAZMapMgr::CreateMap(int32 map_index)
{
	if (auto map_data = GetTableByIndex(UMapData, map_index))
	{
		EMapType map_type = UAZUtility::StringToEnum<EMapType>(map_data->map_type);
		switch(map_type)
		{
		case EMapType::Dungeon:
			return NewObject<UAZDungeon>(this);
		default:
			return NewObject<UAZMapBase>(this);
		}
	}
	return nullptr;
}

void UAZMapMgr::OnRecvMapEnterRequest(const FCG_MAP_ENTER_REQUEST_CMD& recv_msg, bool reserved)
{
	// FIXME 병합시 풀기
	/*if (reserved == false && IsOnLoading() && GetCurrentMapType() != EMapType::Prologue && AZGameInstance->map_channel_info.map_index != recv_msg.map_index)
	{
		ReserveMapEnterRequest(recv_msg);
		return;
	}

	if (AZGameInstance->map_channel_info.map_index == recv_msg.map_index)
	{
		// 같은 냅의 채널 이동
		MapChannelMove(recv_msg.map_index, recv_msg.channel_index);

		// 위치 이동
		if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(AZGameInstance->GetGameMode()))
		{
			if (AAZPlayer_Playable* player = game_mode->GetPlayablePlayer())
			{

			}
		}
	}*/
}

void UAZMapMgr::ReserveMapEnterRequest(const FCG_MAP_ENTER_REQUEST_CMD& msg)
{
	reserved_map_enter_request_.map_index;
	reserved_map_enter_request_.channel_index;
	reserved_map_enter_request_.player_count;
	reserved_map_enter_request_.pos;
	reserved_map_enter_request_.look_at_angle;
	reserved_map_enter_request_.warp_type;
}

void UAZMapMgr::ResetMapEnterRequest()
{
	reserved_map_enter_request_.map_index = 0;
	reserved_map_enter_request_.channel_index = 0;
	reserved_map_enter_request_.player_count = 0;
	reserved_map_enter_request_.pos = FVector::ZeroVector;
	reserved_map_enter_request_.look_at_angle = 0.0f;
	reserved_map_enter_request_.warp_type = EWarpType::None;
}

bool UAZMapMgr::IsMapEnterRequestReserved() const
{
	return reserved_map_enter_request_.map_index > 0;
}

EMapType UAZMapMgr::GetCurrentMapType()
{
	const auto map_data = GetMapData();
	if (map_data == nullptr)
	{
		return EMapType::Max;
	}
	EMapType type = UAZUtility::StringToEnum<EMapType>(map_data->map_type);
	return map_data ? type : EMapType::Max;
}

FVector UAZMapMgr::GetEnterPos() const
{
	return enter_pos_;
}

float UAZMapMgr::GetEnterAngle() const
{
	return enter_angle_;
}

void UAZMapMgr::ConsumeEnterParams(FVector& location, FRotator& rotation)
{
	location = enter_pos_;
	rotation = FRotator(0.f, enter_angle_, 0.f);
	enter_pos_ = FVector::ZeroVector;

}

bool UAZMapMgr::IsInDungeon()
{
	return IsValid(dungeon_info_);
}

void UAZMapMgr::OnBeginDungeon()
{
	if (IsInDungeon())
	{
		/*auto goal_info = dungeon_info_->GetCurrentMainGoalInfo();
		int32 dungeon_info_index = goal_info ? goal_info->id : 0;
		if (auto ui = AZGameInstance->GetHUD()->GetUI<UAZWidget_InGame>(EUIName::AZWidget_InGame))
		{
			//ui->RenewDungeonGoalInfo(dungeon_info_index, true);
		}

		UMapDefineData* data = GetTable(UMapDefineData, 0);
		if (UMapDefineData != nullptr)
		{
			float check_time = UMapDefineData->start_time;
			GetWorld()->GetTimerManager().SetTimer(PartyDungeonAutoBattleStartTimer, this, &UAZMapMgr::DungeonBattleStart, check_time);
		}*/
	}
}

void UAZMapMgr::UpdateStreaming()
{
	if (!game_instance_ || !game_instance_->GetWorld() || !game_instance_->GetWorld()->WorldComposition || !game_instance_->GetPlayer())
	{
		return;
	}

	// FIXME 확인 필요
	/*if (AZGameInstance->GetWorld()->bDisableUpdateStreamingState == false)
	{
		AZGameInstance->GetWorld()->bDisableUpdateStreamingState = true;
	}*/

	if (pause_streaming_ == true)
	{
		return;
	}

	if (current_streaming_state_ == EStreamingState::None)
	{
		RenewLevelStreamingState(false, game_instance_->GetPlayer());
		return;
	}

	switch (current_streaming_state_)
	{
	case EStreamingState::None:
	case EStreamingState::Paused:
		break;
	case EStreamingState::Normal:
	case EStreamingState::Preload:
		UpdateWorldCompositionStreamingState(true);
		break;
	case EStreamingState::Waiting:
	case EStreamingState::Postload:
	case EStreamingState::PrepareCinematic:
		UpdateStreamingCheck();
		break;

	case EStreamingState::OnCinematic:
		UpdateStreamingOnCinematic();
		break;
	}

	if (last_streaming_state_ != current_streaming_state_ && GetName().Contains("Default") == false)
	{
		switch (current_streaming_state_)
		{
		case EStreamingState::None: AZ_LOG("current streaming state : None"); break;
		case EStreamingState::Normal: AZ_LOG("current streaming state : Normal"); break;
		case EStreamingState::Preload: AZ_LOG("current streaming state : Preload"); break;
		case EStreamingState::Waiting: AZ_LOG("current streaming state : Waiting"); break;
		case EStreamingState::Postload: AZ_LOG("current streaming state : Postload"); break;
		case EStreamingState::Paused: AZ_LOG("current streaming state : Paused"); break;
		case EStreamingState::PrepareCinematic: AZ_LOG("current streaming state : "); break;
		case EStreamingState::OnCinematic: AZ_LOG("current streaming state : OnCinematic"); break;
		}
	}

	last_streaming_state_ = current_streaming_state_;
}

void UAZMapMgr::UpdateWorldCompositionStreamingState(bool include_player)
{
	TArray<FVector> locations;
	if (include_player == true)
	{
		FVector location;
		FRotator rot;
		if (character_only_streaming_ == false)
		{
			game_instance_->GetPlayerController()->GetPlayerViewPoint(location, rot);
			locations.Add(location);
		}
		locations.Add(game_instance_->GetPlayer()->GetActorLocation());
	}
	bool include_additional_location = false;
	switch (current_streaming_state_)
	{
	case EStreamingState::Preload:
	case EStreamingState::Waiting:
	case EStreamingState::Postload:
	case EStreamingState::PrepareCinematic:
		include_additional_location = true;
		break;
	}
	if (include_additional_location)
	{
		locations.Add(level_streaming_position_);
	}

	if (locations.Num() > 0)
	{
		if (game_instance_->GetPlayerController() && game_instance_->GetPlayerController()->bCinematicMode)
		{
			game_instance_->GetWorld()->WorldComposition->UpdateStreamingStateCinematic(locations.GetData(), locations.Num());
		}
		else
		{
			game_instance_->GetWorld()->WorldComposition->UpdateStreamingState(locations.GetData(), locations.Num());
		}
	}
}

void UAZMapMgr::UpdateStreamingOnCinematic()
{
	if (game_instance_->GetPlayerController() && game_instance_->GetPlayerController()->bCinematicMode)
	{
		game_instance_->GetWorld()->WorldComposition->UpdateStreamingStateCinematic(cinematic_camera_trajectories_.GetData(), cinematic_camera_trajectories_.Num());
	}
	else
	{
		game_instance_->GetWorld()->WorldComposition->UpdateStreamingState(cinematic_camera_trajectories_.GetData(), cinematic_camera_trajectories_.Num());
	}
}

void UAZMapMgr::UpdateStreamingCheck()
{
	if (game_instance_ == nullptr)
	{
		return;
	}

	if (current_streaming_state_ != Waiting && current_streaming_state_ != Postload && current_streaming_state_ != PrepareCinematic)
	{
		return;
	}

	if (auto* world = game_instance_->GetWorld())
	{
		if (world->WorldComposition == nullptr)
		{
			return;
		}

		bool end_waiting = true;

		auto streaming_levels = world->GetStreamingLevels();

		int count = streaming_levels.Num();

		for (int i = 0; i < count; ++i)
		{
			auto streaming_level = streaming_levels[i];

			if (streaming_level->GetWorldAssetPackageName().Contains(TEXT("_Design_")))
			{
				continue;
			}

			if (streaming_level->ShouldBeLoaded() == false || streaming_level->ShouldBeVisible() == false)
			{
				continue;
			}

			if (current_streaming_state_ == PrepareCinematic)
			{
				// FIXME 확인필요
			}
			else
			{
				if (streaming_level->GetStreamingVolumeBounds().IsValid == false)
				{
					if (streaming_level->bDisableDistanceStreaming == false)
					{
						auto dynamic_streaming_levels = game_instance_->GetGameMode()->GetAsyncLoadableStreamingTile();
						if (dynamic_streaming_levels.Contains(streaming_level) == false)
						{
							continue;
						}
					}
				}
				else
				{
					if (IsInBoundAtLeastOneLocation(streaming_level->GetStreamingVolumeBounds()) == false)
					{
						continue;
					}
				}
			}

			if (streaming_level->IsStreamingStatePending())
			{
				// 하나라도 펜딩 걸려있으면 기다림
				end_waiting = false;
				break;
			}
		}

		if (FShaderPipelineCache::NumPrecompilesRemaining() > 0)
		{
			end_waiting = false;
		}

		if (current_streaming_state_ == Waiting)
		{
			if (end_waiting)
			{
				//FIXME 병합시 확인
				if (/*HasFloor(AZGameInstance->GetPlayer())*/true)
				{
					if (game_instance_->GetGameMode()->HasActorBegunPlay())
					{
						OnFinishStreamingLevelShown();
					}
					else
					{
						current_streaming_state_ = Paused;
						game_instance_->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::OnFinishStreamingLevelShown));
					}
				}
				else
				{
					UpdateWorldCompositionStreamingState(false);
					waiting_max_frame_count_++;
				}
			}
			else
			{
				UpdateWorldCompositionStreamingState(false);
				waiting_max_frame_count_++;
			}

			if (waiting_max_frame_count_ >= 300)
			{
				if (game_instance_->GetGameMode()->HasActorBegunPlay())
				{
					OnFinishStreamingLevelShown();
				}
				else
				{
					current_streaming_state_ = Paused;
					game_instance_->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::OnFinishStreamingLevelShown));
				}
			}
		}
		else if (current_streaming_state_ == Postload)
		{
			check_frame_after_streaming_++;
			if (check_frame_after_streaming_ < 120)
			{
				if (end_waiting == false)
				{
					RenewLevelStreamingState(false, game_instance_->GetPlayer());
				}
				else
				{
					UpdateWorldCompositionStreamingState(true);
				}
			}
			else
			{
				current_streaming_state_ = Normal;
				UpdateWorldCompositionStreamingState(true);
				// 완벽한 완료
			}
		}
		else if (current_streaming_state_ == PrepareCinematic)
		{
			if (end_waiting || waiting_max_frame_count_ >= 300)
			{
				current_streaming_state_ = OnCinematic;
				game_instance_->GetGameMode()->SetGamePaused(false);
			}
			else
			{
				UpdateStreamingOnCinematic();
				waiting_max_frame_count_++;
			}
		}
	}
}

void UAZMapMgr::UpdateLoadingLightHiddenCheck()
{
	if (!game_instance_ || !game_instance_->GetWorld() || !game_instance_->GetWorld()->WorldComposition || !game_instance_->GetPlayer())
	{
		return;
	}

	if (is_loading_ == false)
	{
		return;
	}

	if (game_instance_->GetWorld())
	{
		TArray<AActor*> actors;

		UGameplayStatics::GetAllActorsOfClass(game_instance_->GetWorld(), AActor::StaticClass(), actors);
		for (int i = 0; i < actors.Num(); ++i)
		{
			if (actors[i]->ActorHasTag(TAG_LoadingCheck))
			{
				continue;
			}

			actors[i]->Tags.Emplace(TAG_LoadingCheck);

			if (Cast<AAZActor_LoadingScene>(actors[i]))
			{
				continue;
			}

			if (auto volume = Cast<APostProcessVolume>(actors[i]))
			{
				if (volume->bEnabled)
				{
					volume->bEnabled = false;
					volume->SetActorHiddenInGame(true);

					loading_hidden_volume_actors_.Emplace(volume);
				}
			}

			TArray<UActorComponent*> components;

			actors[i]->GetComponents(ULightComponent::StaticClass(), components, true);
			// 빛 컴포넌트 로딩 넘어가기
			for (int j = 0; j < components.Num(); ++j)
			{
				if (auto light = Cast<ULightComponent>(components[j]))
				{
					light->SetVisibility(false);
					light->SetHiddenInGame(true);

					loading_hidden_light_components_.Emplace(light);
				}
			}

			actors[i]->GetComponents(USkyLightComponent::StaticClass(), components, true);
			for (int j = 0; j < components.Num(); ++j)
			{
				if (auto light = Cast<USkyLightComponent>(components[j]))
				{
					light->SetVisibility(false);
					light->SetHiddenInGame(true);

					loading_hidden_light_components_.Emplace(light);
				}
			}
		}
	}
}

void UAZMapMgr::RenewLevelStreamingState(bool force_flush, /*FIXME 병합시 확인*/ACharacter* player, bool close_loading_scene_on_finish)
{
	UWorld* world = game_instance_->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	if (world->WorldComposition == nullptr)
	{
		return;
	}

	current_streaming_state_ = Waiting;

	waiting_max_frame_count_ = 0;

	close_loading_scene_on_finish_ = close_loading_scene_on_finish;

	if (game_instance_->GetGameMode()->HasActorBegunPlay())
	{
		game_instance_->GetGameMode()->SetGamePaused(true);
	}

	level_streaming_position_ = player ? player->GetActorLocation() : enter_pos_;

	UpdateWorldCompositionStreamingState(false);

	if (force_flush || flush_streaming_)
	{
		FlushStreaming();
		OnFinishStreamingLevelShown();
	}
}

void UAZMapMgr::OnFinishStreamingLevelShown()
{
	UWorld* world = game_instance_->GetWorld();

	if (!world || !world->WorldComposition)
	{
		return;
	}

	// FIXME 병합시 확인
	if (renew_streaming_recursive_count_ < 10 /*&& HasFloor(AZGameInstance->GetPlayer()) == false*/)
	{
		++renew_streaming_recursive_count_;
		RenewLevelStreamingState(false, game_instance_->GetPlayer(), close_loading_scene_on_finish_);
		return;
	}

	if (game_instance_->GetGameMode()->HasActorBegunPlay())
	{
		game_instance_->GetGameMode()->SetGamePaused(false);
	}
	else
	{
		StartPlayManuallyProc();
	}

	TArray<AActor*> characters;
	// FIXME 병합시 캐릭터로 변경하기
	UGameplayStatics::GetAllActorsOfClass(world, ACharacter::StaticClass(), characters);

	for (int32 i = 0; i < characters.Num(); ++i)
	{
		// FIXME 병합시 캐릭터로 변경하기
		/*if (auto character = Cast<AAZCharacter>(characters[i]))
		{
			if (AAZPlayer_Playable* player = Cast<AAZPlayer_Playable>(character))
			{
				character->AdjustStandPosition();
			}
		}*/
	}

	if (close_loading_scene_on_finish_ && IsOnLoading())
	{
		StopCapture();
		game_instance_->GetHUD()->OnFadeInOut(0.5f, 0.5f);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateUObject(this, &UAZMapMgr::ThirdTickAfterStartPlayManuallyProc), 0.5f, false);
	}

	close_loading_scene_on_finish_ = false;

	// FIXME 병합시 확인필요
	/*if (AZGameInstance->GetPlayer())
	{
		AZGameInstance->GetPlayer()->GetAZCharacterMovement_Sync()->SetIgnoreSync(false);
	}*/

	
	// FIXME 병합시 캐릭터 상태 동기화 확인체크하기.
	/*if (ready_server_sync_streaming_)
	{
		if (use_teleport_after_sync_streaming)
		{
			character_state state = AZGameInstance->GetPlayer()->GetCharacterState() == ECharacterState::WarpFly ? ECharacterState::WarpFlyEnd : ECharacterState::WarpEnd;
			AZGameInstance->GetPlayer()->SetCharacterState(state);
		}

		// FIXME 워프 메시지
	}*/

	ready_server_sync_streaming_ = false;
	use_teleport_after_sync_streaming_ = false;

	current_streaming_state_ = Postload;
	check_frame_after_streaming_ = 0;
	renew_streaming_recursive_count_ = 0;
}

void UAZMapMgr::OnRecvWarpRequest(bool is_teleport)
{
	if (game_instance_ == nullptr || game_instance_->GetWorld() == nullptr)
	{
		return;
	}

	if (game_instance_->GetWorld()->WorldComposition)
	{
		ready_server_sync_streaming_ = true;
		if (close_loading_scene_on_finish_ == false)
		{
			close_loading_scene_on_finish_ = is_teleport;
		}
	}
	else
	{
		// FIXME 병합시 체크 필요
		/*if (is_teleport && AZGameInstance->GetPlayer())
		{
			ECharacterState state = AZGameInstance->GetPlayer()->GetCharacterState() == ECharacterState::WarpFly ? ECharacterState::WarpFlyEnd : ECharacterState::WarpEnd;
			AZGameInstance->GetPlayer()->SetCharacterState(state);
		}*/

		//FixMe 워프 End 메시지
		/*if (UAZSocketHolder* socketHolder = AZGameInstance->GetSocketHolder(ESocketHolderType::Gate))
		{
			FCG_MAP_WARP_ENTER_SYN msg;
			socketHolder->SendPacket(&msg);
		}*/
	}
}

void UAZMapMgr::SetPrepareCinmaticStreaming(const TArray<FVector>& cinematic_locations)
{
	if (game_instance_->GetWorld() == nullptr || game_instance_->GetWorld()->WorldComposition == nullptr)
	{
		return;
	}

	cinematic_camera_trajectories_.Empty();

	{
		FVector loc;
		FRotator rot;
		game_instance_->GetPlayerController()->GetPlayerViewPoint(loc, rot);
		cinematic_camera_trajectories_.Add(loc);
		cinematic_camera_trajectories_.Add(game_instance_->GetPlayer()->GetActorLocation());
	}
	cinematic_camera_trajectories_.Append(cinematic_locations.GetData(), cinematic_locations.Num());

	waiting_max_frame_count_ = 0;
	game_instance_->GetGameMode()->SetGamePaused(true);
	current_streaming_state_ = PrepareCinematic;

	UpdateStreamingOnCinematic();
}

void UAZMapMgr::FinishCinematicStreaming()
{
	if (current_streaming_state_ != OnCinematic && current_streaming_state_ != PrepareCinematic)
	{
		return;
	}

	RenewLevelStreamingState(true, game_instance_->GetPlayer());
}

void UAZMapMgr::OpenLoadingScene(int32 map_index)
{
	if (loading_map_index_ == map_index)
	{
		return;
	}

	loading_map_index_ = map_index;

	if (loading_widget_)
	{
		//FixMe 병합시 수정하기
		auto info = GetTable(UModelData);
		const UModelData* model_data = *info.GetData();
		SyncLoadingActorInstance(model_data, map_index);
		loading_widget_->SetLoadingModel(model_data, map_index);
		return;
	}

	// FIXME 경로 확인하기
	FString path = TEXT("/Game/FIXME..");
	UClass* load_class = AZResourceHelper::LoadClassFast<UAZWidget>(path);
	if (load_class)
	{
		loading_widget_ = Cast<UAZWidget_Loading>(UUserWidget::CreateWidgetInstance(*game_instance_, load_class, TEXT("LoadingScene")));
		if (loading_widget_)
		{
			loading_widget_->Init();
			loading_widget_->AddToViewport((int32)EUILayer::Max);
			loading_widget_->OnOpen();

			//FixMe 병합시 수정하기
			auto info = GetTable(UModelData);
			const UModelData* model_data = *info.GetData();
			SyncLoadingActorInstance(model_data, map_index);
			loading_widget_->SetLoadingModel(model_data, map_index);

			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(game_instance_->GetWorld(), AActor::StaticClass(), actors);
			for (int i = 0; i < actors.Num(); ++i)
			{
				actors[i]->Tags.Emplace(TAG_LoadingCheck);
			}

			is_loading_ = true;
		}
		load_class = nullptr;
	}
}

void UAZMapMgr::CloseLoadingScene()
{
	if (loading_widget_ != nullptr)
	{
		return;
	}

	for (int i = 0; i < loading_hidden_volume_actors_.Num(); ++i)
	{
		if (loading_hidden_volume_actors_[i])
		{
			loading_hidden_volume_actors_[i]->bEnabled = true;
			loading_hidden_volume_actors_[i]->SetActorHiddenInGame(false);
		}
	}
	loading_hidden_volume_actors_.Empty();

	for (int i = 0; i < loading_hidden_volume_actors_.Num(); ++i)
	{
		if (IsValid(loading_hidden_light_components_[i]))
		{
			loading_hidden_light_components_[i]->SetVisibility(true);
			loading_hidden_light_components_[i]->SetHiddenInGame(false);
		}
	}
	loading_hidden_light_components_.Empty();

	is_loading_ = false;
	loading_map_index_ = 0;

	ClearLoadingActor();
	loading_widget_->RemoveFromParent();
	loading_widget_->MarkAsGarbage();
	loading_widget_->ConditionalBeginDestroy();
	loading_widget_ = NULL;

	// FIXME 추후 작업하기
	// 로딩끝나고 체크한번
	/*if (IsValid(AZGameInstance->SoundAlarmMgr))
	{
		AZGameInstance->SoundAlarmMgr->CheckAllAlarm(true);
	}*/
}

void UAZMapMgr::StopCapture()
{
	if (IsValid(loading_scene_actor_instance_))
	{
		loading_scene_actor_instance_->StopCapture();
	}
}

void UAZMapMgr::ClearLoadingActor()
{
	
	if (IsValid(loading_scene_actor_instance_))
	{
		loading_scene_actor_instance_->MarkAsGarbage();
		loading_scene_actor_instance_->ConditionalBeginDestroy();
		loading_scene_actor_instance_ = nullptr;
	}
}

bool UAZMapMgr::IsOnLoading() const
{
	return IsValid(loading_widget_) && loading_widget_->IsInViewport();
}

void UAZMapMgr::FlushStreaming()
{
	if (IsValid(game_instance_->GetWorld()) == false)
	{
		return;
	}

	if (game_instance_->GetWorld()->WorldComposition == nullptr)
	{
		return;
	}

	game_instance_->GetWorld()->UpdateLevelStreaming();
	FlushAsyncLoading();
	game_instance_->GetWorld()->UpdateLevelStreaming();

	auto streaming_levels = game_instance_->GetWorld()->GetStreamingLevels();

	while (true)
	{
		bool end_waiting = true;
		for (int i = 0, cnt = streaming_levels.Num(); i < cnt; ++i)
		{
			auto streaming_level = streaming_levels[i];

			if (streaming_level->GetWorldAssetPackageName().Contains(TEXT("_Design_")))
			{
				continue;
			}

			if (streaming_level->ShouldBeLoaded() == false || streaming_level->ShouldBeVisible() == false)
			{
				continue;
			}

			streaming_level->GetStreamingVolumeBounds();
			if (streaming_level->GetStreamingVolumeBounds().IsValid == false)
			{
				if (streaming_level->bDisableDistanceStreaming == false)
				{
					auto dynamic_streaming_levels = game_instance_->GetGameMode()->GetAsyncLoadableStreamingLevel();
					if (dynamic_streaming_levels.Contains(streaming_level) == false)
					{
						continue;
					}
				}
			}
			else
			{
				if (IsInBoundAtLeastOneLocation(streaming_level->GetStreamingVolumeBounds()) == false)
				{
					continue;
				}
			}

			if (streaming_level->IsStreamingStatePending())
			{
				end_waiting = false;
				break;
			}
		}

		if (end_waiting == true)
		{
			break;
		}

		FlushAsyncLoading();
		game_instance_->GetWorld()->UpdateLevelStreaming();
	}
}

bool UAZMapMgr::IsInBoundAtLeastOneLocation(const FBox& bound) const
{
	TArray<FVector> locations = GetStreamingLocations();
	for (int i = 0; i < locations.Num(); ++i)
	{
		if (bound.IsInsideOrOn(locations[i]))
		{
			return true;
		}
	}
	return false;
}

TArray<FVector> UAZMapMgr::GetStreamingLocations() const
{
	bool include_player = true;
	switch (current_streaming_state_)
	{
	case EStreamingState::Waiting:
		include_player = false;
		break;
	}

	bool include_additional_location = false;
	switch (current_streaming_state_)
	{
	case EStreamingState::Preload:
	case EStreamingState::Waiting:
	case EStreamingState::Postload:
	case EStreamingState::PrepareCinematic:
		include_additional_location = true;
		break;
	}

	TArray<FVector> locations;
	if (include_player == true)
	{
		FVector loc;
		FRotator rot;
		game_instance_->GetPlayerController()->GetPlayerViewPoint(loc, rot);
		locations.Add(loc);
		// FIXME 병합시 수정 필요.
		//locations.Add(AZGameInstance->GetPlayer()->GetActorLocation());
	}
	if (include_additional_location)
	{
		// FIXME 병합시 level streaming position 위치 확인하기
		locations.Add(level_streaming_position_);
	}
	return locations;
}

void UAZMapMgr::AddSubLevelFromPackageName(const FName& package_name, UPackage* loaded_package, EAsyncLoadingResult::Type result)
{
	if (GetWorld()->WorldComposition == nullptr)
	{
		TArray<ULevelStreaming*> async_loadable_streaming_list = game_instance_->GetGameMode()->GetAsyncLoadableStreamingLevel();
		for (int32 i = 0; i < async_loadable_streaming_list.Num(); ++i)
		{
			if (async_loadable_streaming_list[i]->GetWorldAssetPackageFName() == package_name)
			{
				async_loadable_streaming_list[i]->SetShouldBeLoaded(true);
				async_loadable_streaming_list[i]->SetShouldBeVisible(true);

				AZ_LOG("[UAZMapMgr::AddSubLevelFromPackageName] %s", *package_name.ToString());
				break;
			}
		}
	}
	else
	{
		TArray<ULevelStreaming*> async_loadable_streaming_list = game_instance_->GetGameMode()->GetAsyncLoadableStreamingLevel();
		for (int32 i = 0; i < async_loadable_streaming_list.Num(); ++i)
		{
			if (async_loadable_streaming_list[i]->GetWorldAssetPackageFName() == package_name)
			{
				bool success;
				auto level = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), async_loadable_streaming_list[i]->GetWorldAsset(), FVector::ZeroVector, FRotator::ZeroRotator, success);
				level->SetShouldBeLoaded(true);

				AZ_LOG("[UAZMapMgr::AddSubLevelFromPackageName] %s (%s)", *package_name.ToString(), (success ? TEXT("Success") : TEXT("Failed")));
				break;
			}
		}
	}
}

bool UAZMapMgr::HasFloor(AAZCharacter* character)
{
	// FIXME 병합시 바닥체크 넣기
	/*float traceZ = character->GetAZCharacterMovement()->ClinetZ_TraceStartZ;
	FVector location = character->GetActorServerLocation();
	FVector start = location + FVector::UpVector * traceZ;
	FVector end = location - FVector::UpVector * traceZ;
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;

	bool result = false;

	result = UKismetSystemLibrary::LineTraceSingle(character, start, end, UEngineTypes::ConvertToTraceType(LH_Trace_Floor), false, actorsToIgnore, EDrawDebugTrace::None, outHit, true);
	if (result)
	{
		return true;
	}

	result = UKismetSystemLibrary::LineTraceSingle(character, start, end, UEngineTypes::ConvertToTraceType(LH_TRACE_SWIM), false, actorsToIgnore, EDrawDebugTrace::None, outHit, true);
	if (result)
	{
		return true;
	}*/

	return false;
}

void UAZMapMgr::StopLoadingActorSceneCapture()
{
	if (IsValid(loading_scene_actor_instance_))
	{
		loading_scene_actor_instance_->StopCapture();
	}
}

void UAZMapMgr::SyncLoadingActorInstance(const UModelData* model_data, int32 map_index)
{
	if (model_data == nullptr)
	{
		return;
	}

	TArray<AActor*> temp_array;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAZActor_LoadingScene::StaticClass(), temp_array);
	if (temp_array.Num() <= 0)
	{
		FVector spawn_location;
		auto loading_data = GetGameSingleton()->table_mgr->GetData<ULoadingData>([map_index](const ULoadingData* check) {
			if (check->map_index == map_index)
			{
				return true;
			}
			return false;
			});

		auto data = *loading_data.GetData();
		if (loading_data.Num() > 0)
		{
			spawn_location = FVector(data->location_x, data->location_y, data->location_z);

		}
		else
		{
			spawn_location = FVector::ZeroVector;
		}

		FString model_bp = (model_data->model_bp.Num() > 0 ? model_data->model_bp[FMath::Rand() % model_data->model_bp.Num()] : TEXT(""));
		// FIXME 병합시 경로 확인하기
		FString loading_scene_path(TEXT("Environment/LoadingScene/"));
		FString load_path = FString(TEXT("/Game/Blueprint/")) + loading_scene_path + model_bp + "." + model_bp + "_C";
		FSoftClassPath path(load_path);
		UClass* load_class = path.TryLoadClass<AAZActor_LoadingScene>();
		loading_scene_actor_instance_ = GetWorld()->SpawnActor<AAZActor_LoadingScene>(load_class, spawn_location, FRotator::ZeroRotator);

		if (IsValid(loading_scene_actor_instance_) == false)
		{
			AZ_LOG("[UAZMapMgr::SyncLoadingActorInstance] Can't find ModelBP(%s)", *model_bp);
			return;
		}

		if (GetWorld()->HasBegunPlay() == false)
		{
			loading_scene_actor_instance_->DispatchBeginPlay();
		}

		EModelType model_type = UAZUtility::StringToEnum<EModelType>(model_data->model_type);
		if (model_type == EModelType::Avatar)
		{
			// FIXME 
			//if (UAvatarInfoRecord* avatarRecord = GetTable(UAvatarTable)->m_AvatarInfoSheet->GetRecord(modelInfo->ModelIndex))
			//{
			//	EClassType modelClassType = (EClassType)(avatarRecord->ClassType);
			//	LoadingSceneActorInstance->SetModelMesh(modelClassType, modelInfo->ModelIndex);
			//}
		}
		else
		{
			loading_scene_actor_instance_->SetCaptureCameraLocation();
		}
		loading_scene_actor_instance_->SetMergeCompVisible(model_type == EModelType::Avatar);
	}
	else if (temp_array.Num() > 0)
	{
		loading_scene_actor_instance_ = Cast<AAZActor_LoadingScene>(temp_array[0]);
	}
}

AAZActor_LoadingScene* UAZMapMgr::GetLoadingSceneActor() const
{
	return loading_scene_actor_instance_;
}
