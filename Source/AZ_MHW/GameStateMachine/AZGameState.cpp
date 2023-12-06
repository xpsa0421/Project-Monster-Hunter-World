// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameStateMachine/AZGameState.h"
#include <Kismet/GameplayStatics.h>
#include "AZ_MHW/GameStateMachine/AZStateMachine.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameMode/AZGameMode_InGame.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZ_MHW/HUD/AZHUD.h"

//FIXME ���ս� ����
#include <GameFramework/Character.h>

UAZGameState::UAZGameState()
{
	is_change_state_ = false;
}

void UAZGameState::Init(EGameState init_state_id)
{
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());

	state_id_ = init_state_id;

	current_state_name_ = GetName();
}

void UAZGameState::OnStart()
{

}

void UAZGameState::DestroyState()
{
	// Destroy
}

void UAZGameState::OnGameMsg(FAZGameMsg* game_msg)
{

}

AAZGameMode_InGame* UAZGameState::GetOwnerGameMode()
{
	if (GetStateMachine())
	{
		return Cast<AAZGameMode_InGame>(GetStateMachine()->GetOuter());
	}
	return nullptr;
}

UAZStateMachine* UAZGameState::GetStateMachine() const
{
	return Cast<UAZStateMachine>(GetOuter());
}

bool UAZGameState::TransitionState(float delta_seconds)
{
	if (is_change_state_)
	{
		if (state_transition_.time > 0.0f)
		{
			state_transition_.time -= delta_seconds;

			if (state_transition_.time <= 0.0f)
			{
				is_change_state_ = false;
				state_transition_.time = 0.0f;
				GetStateMachine()->SetState(state_transition_.state_id);
				return true;
			}
		}
		else
		{
			is_change_state_ = false;
			GetStateMachine()->SetState(state_transition_.state_id);
			return true;
		}
	}
	return false;
}

void UAZGameState::ClearTransition()
{
	is_change_state_ = false;

	state_transition_.state_id = EGameState::None;
	state_transition_.time = 0.0f;
}

void UAZGameState::SetState(EGameState state_id, float time, bool immediately)
{
	is_change_state_ = true;

	state_transition_.state_id = state_id;
	state_transition_.time = time;

	if (immediately == true)
	{
		TransitionState(GetWorld()->GetDeltaSeconds());
	}
}

bool UAZGameState::SetStateImmediately(EGameState state_id)
{
	SetState(state_id, 0.0f, true);

	bool check =  (GetStateMachine()->GetCurStateID() == state_id);
	return check;
}

bool UAZGameState::IsBlockChangeTo(EGameState state_id)
{
	if (block_states_.Num() <= 0)
	{
		return false;
	}

	if (block_states_.Contains(state_id) == false)
	{
		return false;
	}

	return true;
}

ACharacter* UAZGameState::GetPlayer()
{
	auto* player = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (player == nullptr)
	{
		player = game_instance_->GetPlayer();
	}
	return player;
}

APlayerController* UAZGameState::GetPlayerController()
{
	return Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

AAZHUD* UAZGameState::GetHUD()
{
	return GetPlayerController() ? Cast<AAZHUD>(GetPlayerController()->GetHUD()) : nullptr;
}
