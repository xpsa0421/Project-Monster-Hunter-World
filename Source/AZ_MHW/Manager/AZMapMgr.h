// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/Manager/AZSingletonObject.h"
#include "AZ_MHW/ClientMsg/AZMsgObject.h"
#include "AZ_MHW/CommonSource/Define/MapStruct.h"
#include "AZ_MHW/CommonSource/Define/LoginStruct.h"
#include "AZ_MHW/CommonSource/Table/MapData.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/Map/AZMapBase.h"
#include "AZMapMgr.generated.h"

USTRUCT()
struct FSavedAutoMoveData
{
	GENERATED_BODY()

	int32 map_index;
	EAutoMove auto_move_type;
	FVector target_location;

	FSavedAutoMoveData()
	{
		Reset();
	}
	void Reset()
	{
		map_index = -1;
		auto_move_type = EAutoMove::None;
		target_location = FVector::ZeroVector;
	}
};

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZMapMgr : public UAZSingletonObject, public FTickableGameObject, public IAZMsgObject
{
	GENERATED_BODY()
public:
	enum EStreamingState
	{
		None,
		Normal,
		Preload,
		Waiting,
		Postload,
		Paused,
		PrepareCinematic,
		OnCinematic
	};

	struct FMapMoveRequest
	{
		int32 object_serial;
		int32 map_index;
		FVector pos;
		float look_at_angle;
		uint8 channel_index;
		int32 player_count;
		EWarpType warp_type;
	};

private:
	float loop_timer_;
	bool is_map_entered_;
	bool is_portal_moving_;
	FVector enter_pos_;
	float enter_angle_;

	FDelegateHandle post_load_map_delegate_handle_;
	FVector level_streaming_position_;

	uint8 is_loading_ : 1;
	uint8 flush_streaming_ : 1;
	int32 waiting_max_frame_count_;
	uint8 ready_server_sync_streaming_ : 1;
	uint8 use_teleport_after_sync_streaming_ : 1;
	EStreamingState last_streaming_state_;
	EStreamingState current_streaming_state_;
	EForceKick reserved_force_kick_;
	uint8 same_map_enter_ready_ : 1;
	FSavedAutoMoveData saved_auto_move_data_;
	int32 loading_map_index_;
	int32 disconnected_caching_map_index_;

	int32 check_frame_after_streaming_;
	int32 renew_streaming_recursive_count_;
	uint8 pause_streaming_ : 1;
	uint8 character_only_streaming_ : 1;
	uint8 close_loading_scene_on_finish_ : 1;

	TArray<FVector> cinematic_camera_trajectories_;

	UPROPERTY() class UAZWidget_Loading* loading_widget_;
	UPROPERTY() class AAZActor_LoadingScene* loading_scene_actor_instance_;
	UPROPERTY() TArray<USceneComponent*> loading_hidden_light_components_;
	UPROPERTY() TArray<class APostProcessVolume*> loading_hidden_volume_actors_;

	FMapMoveRequest reserved_map_enter_request_;

	UPROPERTY() class UAZMapBase* current_map_;
	UPROPERTY() class UAZDungeon* dungeon_info_;

public:
	UAZMapMgr();
	virtual ~UAZMapMgr();

	virtual void Init() override;
	virtual void LobbyEnterAck() override;
	virtual void Tick(float delta_time) override;
	virtual void OnGameMsg(struct FAZGameMsg* new_msg) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override { return TStatId(); }

	void MapEnter(uint32 map_rec_key, FVector enter_pos, float enter_angle);
	void SameMapEnterPorc(FVector enter_pos, float enter_angle);
	UFUNCTION() 
	void PostLoadMapFunction(class UWorld* word);
	void MapChannelMove(int32 map_rec_key, int32 channel_index);
	void OnMapEnter_Ack();
	void OnMapExit();
	bool OnRequestMapOut();
	void SendMapEnter();
	void StartPlayManuallyProc();
	void MapDestroy();
	void MapExit_Syn();

	UFUNCTION() void FirstTickAfterStartPlayManuallyProc();
	UFUNCTION() void SecondTickAfterStartPlayManuallyProc();
	UFUNCTION() void ThirdTickAfterStartPlayManuallyProc();

	//FIXME 병합시 추가하기
	//void AppearObject(...);
	//void DisappearObject(const TArray<int32>& array_object_serial);
	void ProcessDisappearObject(int32 object_serial);
	bool IsDeadInsteadDisappear(int32 object_serial);

	bool IsInMap() const { return is_map_entered_ /*&& GetMapRecord();*/; }
	bool IsInMapType(EMapType type) const;

	const UAZMapBase* GetCurMap() const { return current_map_; }
	const UMapData* GetMapData() const {
		return current_map_ ? current_map_->GetMapData() : nullptr;
	}
	int32 GetMapIndex() const;
	class UAZMapBase* CreateMap(int32 map_index);

	bool IsInDungeon();
	void OnBeginDungeon();

	void OnRecvMapEnterRequest(const struct FCG_MAP_ENTER_REQUEST_CMD& recv_msg, bool reserved);
	void ReserveMapEnterRequest(const struct FCG_MAP_ENTER_REQUEST_CMD& msg);
	void ResetMapEnterRequest();
	bool IsMapEnterRequestReserved() const;

	EMapType GetCurrentMapType();
	FVector GetEnterPos() const;
	float GetEnterAngle() const;
	void ConsumeEnterParams(FVector& location, FRotator& rotation);

	void UpdateStreaming();
	void UpdateWorldCompositionStreamingState(bool includePlayer);
	void UpdateStreamingOnCinematic();

	void UpdateStreamingCheck();
	void UpdateLoadingLightHiddenCheck();
	void RenewLevelStreamingState(bool force_flush = false, class ACharacter* playe = nullptr, bool close_loading_scene_on_finish = false);
	UFUNCTION() void OnFinishStreamingLevelShown();
	void OnRecvWarpRequest(bool is_teleport);
	void SetPrepareCinmaticStreaming(const TArray<FVector>& cinematic_locations);
	void FinishCinematicStreaming();

	void OpenLoadingScene(int32 map_index);
	void CloseLoadingScene();
	void StopCapture();

	bool IsOnLoading() const;

	void FlushStreaming();

public:
	void StopLoadingActorSceneCapture();
	void SyncLoadingActorInstance(const class UModelData* model_data, int32 map_index);
	void ClearLoadingActor();
	class AAZActor_LoadingScene* GetLoadingSceneActor() const;

public:
	void AddSubLevelFromPackageName(const FName& package_name, UPackage* loaded_package = nullptr, EAsyncLoadingResult::Type result = EAsyncLoadingResult::Type::Succeeded);

private:
	TArray<FVector> GetStreamingLocations() const;
	bool IsInBoundAtLeastOneLocation(const FBox& bound) const;

	//FIXME 병합시 확인필요
	bool HasFloor(class AAZCharacter* character);

protected:
	UPROPERTY() class UAZGameInstance* game_instance_;

};
