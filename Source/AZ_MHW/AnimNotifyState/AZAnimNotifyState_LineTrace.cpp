// Copyright Team AZ. All Rights Reserved.


#include "AZAnimNotifyState_LineTrace.h"

#include "CollisionDebugDrawingPublic.h"
#include "AZ_MHW/Actor/AZSocketActor.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"

UAZAnimNotifyState_LineTrace::UAZAnimNotifyState_LineTrace()
{
	//CDO Default Setting
	is_single_target_ = true;
	is_hit_once_ = true;
	hit_tick_second_ = 0.1f;

	//TEMP
	is_already = true;
}

void UAZAnimNotifyState_LineTrace::NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	float total_duration, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyBegin(mesh_comp, animation, total_duration, event_reference);

	if(const auto player_actor = Cast<AAZPlayer>(mesh_comp->GetOwner()))
	{
		//플레이어 무기타입에 따라 소켓가져오는거 달라지게끔.
		FName weapon_socket_name;
		if(main_socket_actor_name_ == NAME_None)
		{
			weapon_socket_name = TEXT("MainHand");
		}
		else
		{
			weapon_socket_name = main_socket_actor_name_;
		}
	
		if(const auto found_socket_actor = player_actor->character_sockets_map_.Find(weapon_socket_name))
		{
			main_socket_actor_ = (*found_socket_actor);
		}

		//TODO: 만약 쌍검이라면
		// if(sub_socket_actor_name_ == NAME_None)
		// {
		// 	weapon_socket_name = TEXT("SubHand");
		// }
		// else
		// {
		// 	weapon_socket_name = sub_socket_actor_name_;
		// }
		//
		// if(const auto found_socket_actor = player_actor->character_sockets_map_.Find(weapon_socket_name))
		// {
		// 	main_socket_actor_ = (*found_socket_actor);
		// }
		
	}
}

void UAZAnimNotifyState_LineTrace::NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	float frame_delta_time, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyTick(mesh_comp, animation, frame_delta_time, event_reference);
	
	
}

void UAZAnimNotifyState_LineTrace::NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyEnd(mesh_comp, animation, event_reference);

	is_already = true;
}

//TEMP
void UAZAnimNotifyState_LineTrace::SetDisable()
{
	is_already = false;
}

