// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZStateMachine.generated.h"

/**
 * 
 */
class UAZGameState;
enum class EGameState : uint8;
UCLASS(Blueprintable)
class AZ_MHW_API UAZStateMachine : public UObject
{
	GENERATED_BODY()
	friend class UAZGameState;
public:
	UAZStateMachine();

	virtual void OnCreate();
	virtual void OnStart();
	virtual void ReleaseSubobjects();
	virtual void Tick(float delta_seconds);

	void ClearStates();
	void SetState(EGameState state_id, bool restore_prev = false);
	void SetPreState();
	void ClearPrevState();
	void ChangeState(EGameState state_id, float delay_time = 0.0f, bool immediately = false);

	EGameState GetStartState() const;
	EGameState GetCurStateID() const;
	void SetStartState(const EGameState state);

	template<typename T>
	T* GetCurState() const
	{
		T* state = Cast<T>(cur_state);
		return state;
	}

	UAZGameState* GetState(EGameState state_id);

	template<typename T>
	T* GetState(EGameState state_id)
	{
		return Cast<T>(GetState(state_id));
	}

	void OnGameMsg(struct FAZGameMsg* game_msg);

private:
	UPROPERTY(EditAnywhere, Category = "GameFSM")
	EGameState start_state;
	UPROPERTY(EditAnywhere, Category = "GameFSM")
	TMap<EGameState, TSubclassOf<UAZGameState>> state_classes;


protected:
	UPROPERTY()
	class AAZGameMode* game_mode;
	UPROPERTY()
	class UAZGameState* cur_state;
	UPROPERTY()
	TMap<EGameState, UAZGameState*> game_state_map;
	UPROPERTY()
	bool is_state_changing = false;

};
