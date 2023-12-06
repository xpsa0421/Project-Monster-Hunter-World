// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZGameState.generated.h"

struct FGameStateTransition
{
	float time;
	EGameState state_id;
	FGameStateTransition()
		: time(0.0f)
	{}
};

struct FGameStateContext
{
	EGameState prev_state_id;
	EGameState next_state_id;
};

UCLASS(Abstract, Blueprintable)
class AZ_MHW_API UAZGameState : public UObject
{
	GENERATED_BODY()

public:
	UAZGameState();
	virtual void Init(EGameState init_state_id);

	virtual void OnStart();
	// ���� �����Լ� ��ũ��(��ġ ���� �Լ��� ������ �Լ������� ������ �ִ� ����)
	virtual void BeginState() PURE_VIRTUAL(UAZGameState::BeginState(),);
	virtual void EndState() PURE_VIRTUAL(UAZGameState::EndState(), );
	virtual void Update(float delta_seconds) PURE_VIRTUAL(UAZGameState::Update(),);
	virtual void DestroyState();
	virtual void OnGameMsg(struct FAZGameMsg* game_msg);
	virtual void RestoredState() {}

public:
	class AAZGameMode_InGame* GetOwnerGameMode();
	class UAZStateMachine* GetStateMachine() const;
	EGameState GetStateID() const { return state_id_; }

	bool TransitionState(float delta_seconds);
	void ClearTransition();

	void SetState(EGameState state_id, float time = 0.0f, bool immediately = false);
	bool SetStateImmediately(EGameState state_id);

	bool IsBlockChangeTo(EGameState state_id);

protected:
	// FIXME (���ս� ���� �ʿ�)
	ACharacter* GetPlayer();
	// FIXME (���ս� ���� �ʿ�)
	APlayerController* GetPlayerController();
	class AAZHUD* GetHUD();

public:
	FGameStateContext state_context;

private:
	bool is_change_state_ = false;
	FGameStateTransition state_transition_;
	EGameState state_id_;

protected:
	TArray<EGameState> block_states_;
	UPROPERTY() FString current_state_name_;

protected:
	UPROPERTY() class UAZGameInstance* game_instance_;
};
