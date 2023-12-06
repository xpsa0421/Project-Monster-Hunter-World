// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimInstance/AZAnimInstance_Player.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZPlayerMgr.h"
#include "Character/AZCharacter.h"
#include "Character/Player/AZPlayer.h"
#include "Character/Player/AZPlayer_Origin.h"
#include "PlayerState/AZPlayerState_Client.h"

UAZAnimInstance_Player::UAZAnimInstance_Player()
{
	//기본설정은 몽타주를 재생하는 것.
	is_montage_ = true;
	is_rotation_ = false;
	can_input_control_ = true;
	current_anim_play_rate_ = 2.f;
	anim_state_bitmask_ = 0;
	anim_command_bitmask_ = 0;
	
	handle_animation_transition_trigger_.BindLambda(
		[this](UAZAnimInstance* T) -> bool
		{
			return false;
		} );
}
void UAZAnimInstance_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//플레이어 에셋매니저에서 Base 몽타주를 가져와서 할당
	if(const auto& anim_montage_asset = UAZGameSingleton::instance()->player_mgr_->GetMontage(TEXT("Base")))
	{
		current_montage_name_ = "Base";
		current_anim_montage_ = anim_montage_asset;
		Montage_Play(current_anim_montage_, current_anim_play_rate_);
	}
}

void UAZAnimInstance_Player::NativeUpdateAnimation(float delta_seconds)
{
	Super::NativeUpdateAnimation(delta_seconds);
	const auto& player = Cast<AAZPlayer>(owner_);
	if(player == nullptr) return;
	if(player->player_character_state_ == nullptr) return;
	const auto& player_state = player->player_character_state_;

	//입력값을 받을 수 있을때만 입력값 갱신
	if(can_input_control_)
	{
		input_direction_ = player_state->action_state_.input_direction.Yaw;
		input_bitmask_= (player_state->action_state_.input_bitmask);
	}
	else
	{
		input_bitmask_= 0;
		//입력 방향값은 초기화 안한다.
	}
	
	//전투가 불가능하면 input_bitmask에서 공격인식마스크 제거
	if(player_state->character_state_.bit_can_battle)
	{
		const int32 attack_bitmask = (16|32);
		input_bitmask_ &= ~attack_bitmask;
	}
	
	////////////////////////////////////////////////////////////
	if(is_rotation_)//애니메이션이 회전보간을 사용할때(이동, 정지회전)
	{
		current_forward_direction_ = player_state->character_state_.character_direction;
     	next_forward_direction_ = player_state->action_state_.input_direction;
		
		const FQuat next_quaternion(next_forward_direction_);
		const FQuat current_quaternion(current_forward_direction_);//player class
		owner_->GetRootComponent()->SetWorldRotation(FQuat::Slerp(next_quaternion, current_quaternion,0.8));

		//회전 보간 종료
		if(FMath::Abs(next_forward_direction_.Yaw) - FMath::Abs(current_forward_direction_.Yaw) < 0.1f)
		{
			is_rotation_ = false;
		}
	}
	////////////////////////////////////////////////////////////
	if(should_transition_)//즉시 전환 or 지연 전환()
	{
		OnAnimationTransitionTrigger();
		should_transition_ = false;
	}
	////////////////////////////////////////////////////////////
	else//델리게이트 조건 전환
	{
		//매업데이트 종료조건 델리게이트를 체크한다.
		//기본은 false만 반환하여 특수 종료조건을 처리하지 않는다
		if(handle_animation_transition_trigger_.IsBound())
		{
			if(handle_animation_transition_trigger_.Execute(this))
			{
				OnAnimationTransitionTrigger();
			}
		}
	}
	////////////////////////////////////////////////////////////
}

void UAZAnimInstance_Player::OnAnimationTransitionTrigger()
{
	if(is_montage_)//몽타주 모드
	{
		if(next_montage_name_ != NAME_None)//몽타주 변경
		{
			if(current_montage_name_ != next_montage_name_)//동일 몽타주일시 무시
			{
				if(const auto& next_montage_asset = UAZGameSingleton::instance()->player_mgr_->GetMontage(next_montage_name_))
				{
					Montage_Stop(1.f, current_anim_montage_);
					Montage_Play(next_montage_asset, current_anim_play_rate_);

					current_anim_montage_ = next_montage_asset;
					
					current_montage_name_ = next_montage_name_;
					current_section_name_ = TEXT("Default");//일단 기본 섹션으로 지정
				}
			}
			//몽타주 설정 초기화
			next_montage_name_ = NAME_None;
		}
		
		if(next_section_name_ != NAME_None)//섹션 변경도 있다면
		{
			if(current_section_name_ != next_section_name_)//동일 섹션이 아니라면 실행
			{
				if(current_anim_montage_->IsValidSectionName(next_section_name_))//유효한 섹션 이름 체크
				{
					current_section_name_ = next_section_name_;

					//점프 투 섹션 엔드를 해야 노티파이들과 원활하게 작동되는데(끝지점에서 다른 체크가 필요한 애니메이션의 경우),
					//섹션의 끝 시점의 씨퀀스 포즈가 출력되서... 튀는 모습이 연출되어서 제거
					//Montage_JumpToSectionsEnd(current_section_name_, current_anim_montage_);
					
					Montage_JumpToSection(current_section_name_, current_anim_montage_);
				}
			}
			//섹션 설정 초기화
			next_section_name_ = NAME_None;
		}
		else
		{
			current_section_name_ = TEXT("Default");//이름을 디폴트로
			Montage_JumpToSection(current_section_name_, current_anim_montage_);
		}
	}
	else//시퀀스 모드
	{
		if(current_sequence_name_ != next_sequence_name_)
		{
			if(const auto& next_sequence_asset = UAZGameSingleton::instance()->player_mgr_->GetSequence(next_sequence_name_))
			{
				current_anim_sequence_ = next_sequence_asset;
				current_sequence_name_ = next_sequence_name_;

				next_sequence_name_ = NAME_None;
			}
		}
	}
	
	//전환 조건 트리거 초기화
	handle_animation_transition_trigger_.Unbind();
	handle_animation_transition_trigger_.BindLambda([this](UAZAnimInstance* T) -> bool {return false;});
}

void UAZAnimInstance_Player::SetDirectAnimSequence(FName sequence_name)
{
	is_montage_ = false;
	if(current_sequence_name_ != sequence_name)
	{
		should_transition_ = true;
		next_sequence_name_= sequence_name;
	}
}
void UAZAnimInstance_Player::SetDirectAnimMontage(FName montage_name)
{
	is_montage_ = true;
	if(current_montage_name_ != montage_name)
	{
		should_transition_ = true;
		next_montage_name_ = montage_name;
	}
}
void UAZAnimInstance_Player::SetDirectSection(FName section_name)
{
	is_montage_ = true;
	if(current_section_name_ != section_name)
	{
		should_transition_ = true;
		next_section_name_ = section_name;
	}
}

void UAZAnimInstance_Player::SetNextAnimMontage(FName montage_name)
{
	is_montage_ = true;
	next_montage_name_ = montage_name;
}
void UAZAnimInstance_Player::SetNextSection(FName section_name)
{
	is_montage_ = true;
	next_section_name_ = section_name;
}

void UAZAnimInstance_Player::PauseAnimation()
{
	if(is_montage_)
	{
		Montage_Pause(current_anim_montage_);
	}
	else
	{
		//정지
		SetAnimPlayRate(0);
	}
}
void UAZAnimInstance_Player::ResumeAnimation()
{
	if(is_montage_)
	{
		Montage_Resume(current_anim_montage_);
	}
	else
	{
		SetAnimPlayRate(current_anim_play_rate_);
	}
}
void UAZAnimInstance_Player::SetAnimPlayRate(int32 play_rate)
{
	float play_rate_permyriad;
	
	if(play_rate < 1)
	{
		play_rate_permyriad = 0;
	}
	else
	{
		play_rate_permyriad = play_rate/10000.f;
	}
	
	current_anim_play_rate_ = play_rate_permyriad;
	if(Montage_IsPlaying(current_anim_montage_))
	{
		Montage_SetPlayRate(current_anim_montage_, current_anim_play_rate_);
	}
	//SequencePlayer의 AnimPlayRate는 AnimGraph에서 바인딩되어서 실행
}

void UAZAnimInstance_Player::AnimNotify_OnUseItem()
{
	if(const auto& player = Cast<AAZPlayer_Origin>(owner_))
	{
		player->AnimNotify_OnUseItem();
	}
}


bool UAZAnimInstance_Player::CorrectInputBitMask(int32 bitmask) const
{
	return ((input_bitmask_ & ~bitmask) > 0);
}

bool UAZAnimInstance_Player::CheckInputBitMask(int32 bitmask) const
{
	return ((input_bitmask_ & bitmask) > 0);
}
bool UAZAnimInstance_Player::CheckMontage(FName montage_name) const
{
	return (current_montage_name_ == montage_name);
}
bool UAZAnimInstance_Player::CheckSection(FName section_name) const
{
	return (current_section_name_ == section_name);
}
void UAZAnimInstance_Player::SetAnimationProperty(bool is_rotation, bool can_control)
{
	is_rotation_ = is_rotation;
	can_input_control_ = can_control;
}

void UAZAnimInstance_Player::RecoverStamina(int recover_stamina)
{
	const auto& player = Cast<AAZPlayer>(owner_);
	if(player->player_character_state_->character_state_.current_stamina > player->player_character_state_->character_state_.max_stamina)
	{
		player->player_character_state_->character_state_.current_stamina = player->player_character_state_->character_state_.max_stamina;
		return;
	}
	player->player_character_state_->character_state_.current_stamina += recover_stamina;
}

bool UAZAnimInstance_Player::CheckCostAnimation()
{
	const auto& player = Cast<AAZPlayer>(owner_);
	if(player->player_character_state_->character_state_.current_stamina>30)
	{
		player->player_character_state_->character_state_.current_stamina -= 30;
		return true;
	}
	return false;
}

void UAZAnimInstance_Player::ForceImmediatelyRotate()
{
	is_rotation_ = false;
	FRotator goal_direction = FRotator(0,(input_direction_),0);
	owner_->GetRootComponent()->SetWorldRotation(goal_direction);
}


//montage_name
//datatable.Find(montage_name)
//section_name_map
//section_name_map.Find(section_name)
//result->animation_hash
//animation_hash.table(input_bitmask)

//result
//next montage_name -> section_name

//animation extra type =
//rotate_animation 회전
//link_animation 연계(종료시 입력체크후 분기)//노티파이 사용.
////attack_animation 공격(공격 노티파이 스테이트)//애니메이션 에디터에서 손봐야되는 부분
//animation	

//기본, 엔트리, 링커
//루프, 홀드
//int32 montage_bitmask = UAZGameSingleton::instance()->player_mgr_->GetMontageBitMask(current_montage_name, current_section_name);


