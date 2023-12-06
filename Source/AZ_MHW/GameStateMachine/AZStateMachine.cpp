// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameStateMachine/AZStateMachine.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/CommonSource/AZGameMsg.h"
#include "AZ_MHW/GameStateMachine/AZGameState.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"

UAZStateMachine::UAZStateMachine()
{
	start_state = EGameState::None;
	cur_state = nullptr;
}

void UAZStateMachine::OnCreate()
{
	// ���Ӹ�尡 ������ ����
	game_mode = CastChecked<AAZGameMode>(GetOuter());

	for (const TPair<EGameState, TSubclassOf<class UAZGameState>>& kvp : state_classes)
	{
		if (kvp.Key == EGameState::None)
		{
			continue;
		}

		if (kvp.Value != nullptr)
		{
			FStaticConstructObjectParameters params(kvp.Value);
			params.Outer = this;
			UAZGameState* new_game_state = Cast<UAZGameState>(StaticConstructObject_Internal(params));
			new_game_state->Init(kvp.Key);

			game_state_map.FindOrAdd(kvp.Key) = new_game_state;
		}
	}
}

void UAZStateMachine::OnStart()
{
	for (const TPair<EGameState, UAZGameState*>& kvp : game_state_map)
	{
		if (kvp.Key == EGameState::None)
		{
			continue;
		}
		if (kvp.Value != nullptr)
		{
			kvp.Value->OnStart();
		}
	}

	if (start_state == EGameState::None)
	{
		start_state = EGameState::GameDefault;
	}

	SetState(start_state);
}

void UAZStateMachine::ReleaseSubobjects()
{
	ClearStates();
	cur_state = nullptr;
}

void UAZStateMachine::Tick(float delta_seconds)
{
	if (cur_state != nullptr)
	{
		if (cur_state->TransitionState(delta_seconds))
		{
			return;
		}

		cur_state->Update(delta_seconds);
	}
}

void UAZStateMachine::ClearStates()
{
	for (TPair<EGameState, UAZGameState*>& kvp : game_state_map)
	{
		kvp.Value->DestroyState();
		kvp.Value->ConditionalBeginDestroy(); // ���� ī��Ʈ�� 0���� �������� ���� ȣ���� �������� �޸𸮿��� ���� ����
		kvp.Value = nullptr;
	}
	game_state_map.Reset();
}

void UAZStateMachine::SetState(EGameState state_id, bool restore_prev)
{
	if (is_state_changing == false)
	{
		if (cur_state != nullptr && cur_state->IsBlockChangeTo(state_id))
		{
			// block
		}
		else
		{
			UAZGameState* new_state = game_state_map.FindRef(state_id);
			if (cur_state == new_state)
			{
				return;
			}

			is_state_changing = true;

			if (cur_state != nullptr)
			{
				cur_state->state_context.next_state_id = new_state->GetStateID();
				cur_state->EndState();
				AZ_LOG("EndState(%s)", *cur_state->GetName());
			}

			if (new_state != nullptr)
			{
				if (!restore_prev && cur_state)
				{
					new_state->state_context.prev_state_id = cur_state->GetStateID();
				}

				if (state_id == EGameState::GameDefault)
				{
					ClearPrevState();
				}
			}

			cur_state = new_state;
			if (cur_state != nullptr)
			{
				AZ_LOG("BeginState(%s)", *cur_state->GetName());
				cur_state->ClearTransition();
				cur_state->BeginState();
				if (restore_prev)
				{
					cur_state->RestoredState();
				}
			}

			is_state_changing = false;
		}
	}
}

void UAZStateMachine::SetPreState()
{
	if (cur_state == nullptr)
	{
		return;
	}

	if (cur_state)
	{
		EGameState state_id = cur_state->state_context.prev_state_id;
		if (state_id != EGameState::None)
		{
			//if (state_id == EGame)
			SetState(state_id, true);
		}
		else
		{
			SetState(EGameState::GameDefault, true);
		}
	}
}

void UAZStateMachine::ClearPrevState()
{
	for (TPair<EGameState, UAZGameState*>& kvp : game_state_map)
	{
		if (kvp.Value)
		{
			kvp.Value->state_context.prev_state_id = EGameState::None;
		}
	}
}

void UAZStateMachine::ChangeState(EGameState state_id, float delay_time, bool immediately)
{
	if (game_state_map.Contains(state_id) == false)
	{
		return;
	}

	if (cur_state)
	{
		cur_state->SetState(state_id, 0.0f, immediately);
	}
	else
	{
		SetState(state_id);
	}
}

EGameState UAZStateMachine::GetStartState() const
{
	return start_state;
}

EGameState UAZStateMachine::GetCurStateID() const
{
	return cur_state ? cur_state->GetStateID() : EGameState::None;
}

void UAZStateMachine::SetStartState(const EGameState state)
{
	start_state = state;
}

void UAZStateMachine::OnGameMsg(FAZGameMsg* game_msg)
{
	//FIXME ��ܿ��� ó���� �޽��� �߰��ϱ�
	//��ܿ��� 
	if (cur_state != nullptr)
	{
		cur_state->OnGameMsg(game_msg);
	}
}

UAZGameState* UAZStateMachine::GetState(EGameState state_id)
{
	UAZGameState** state = game_state_map.Find(state_id);
	if (state != nullptr && (*state) != nullptr)
	{
		return *state;
	}
	return nullptr;
}