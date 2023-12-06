// Copyright Team AZ. All Rights Reserved.


#include "AZ_AnimNotify_AttachSocket.h"

#include "Actor/AZSocketActor.h"
#include "Character/Player/AZPlayer.h"

void UAZ_AnimNotify_AttachSocket::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);
	
	if(const auto& player = Cast<AAZPlayer>(mesh_comp->GetOwner()))
	{
		if(const auto& socket_actor = player->character_sockets_map_.Find(socket_actor_name_))
		{
			(*socket_actor)->SetSocketComponent(to_socket_name_);
		}
	}
}
