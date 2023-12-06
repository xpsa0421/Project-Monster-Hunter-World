// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZ_AnimNotifyState_HoldLink.generated.h"

class AAZPlayer;
class UAZAnimInstance_Player;
/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZ_AnimNotifyState_HoldLink : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	/** */
	virtual void NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration, const FAnimNotifyEventReference& event_reference) override;
	/** */
	virtual void NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float frame_delta_time, const FAnimNotifyEventReference& event_reference) override;
	/** */
	virtual void NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

	UPROPERTY() UAZAnimInstance_Player* owner_anim_instance_;
	UPROPERTY() AAZPlayer* owner_;
	
	UPROPERTY(EditAnywhere)
	int32 input_bitmask_;
	UPROPERTY(EditAnywhere)
	FName target_montage_name_;
	UPROPERTY(EditAnywhere)
	FName target_section_name_;

	bool is_next_link_;
};
