// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AIController.h"
#include "AZAIController.generated.h"

class UBehaviorTreeComponent;
class UBehaviorTree;
class UAISenseConfig_Sight;
class AAZMonster;
enum class ECharacterState : uint8;

UCLASS(Blueprintable)
class AZ_MHW_API AAZAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAZAIController();
	virtual void OnPossess(APawn* const pawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float delta_seconds) override;
	
private:	
	// Sight perception
	UFUNCTION() void SetUpPerceptionSystem();
	UFUNCTION() void OnPlayerDetected(AActor* actor, FAIStimulus const stimulus);

	// Event listeners
	
	// Team Agent Interface overrides
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& other_actor) const override;

	// Set up default properties
	void SetUpProperties();
	void SetUpBehaviorTree();

	// For MoveTo Task
	FAIRequestID GetNewMoveRequestID();

public:
	UFUNCTION() void OnEnterCombat();

public:
	// Blackboard value setter functions
	void SetBlackboardValueAsFloat(FName bb_key_name, float bb_value);
	void SetBlackboardValueAsBool(FName bb_key_name, bool bb_value);
	void SetBlackboardValueAsEnum(FName bb_key_name, uint8 enum_idx);
	void SetBlackboardValueAsObject(FName bb_key_name, UObject* bb_value);

	// For MoveTo Task
	FORCEINLINE FAIRequestID GetMoveRequestID() const { return active_move_request_id_; };
	FPathFollowingRequestResult MoveToLocation(const FVector& location);
	bool HasReachedLocation(const FVector& location) const;
	
protected:
	TObjectPtr<AAZMonster> owner_;
	TObjectPtr<UAISenseConfig_Sight> sight_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") TObjectPtr<UBehaviorTree> behavior_tree_;
	
	// For MoveTo task
	uint32 next_request_id_;
	FAIRequestID active_move_request_id_;
	FVector target_location_;
	float acceptance_radius_;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ")	FMonsterSightConfigs sight_configs_;
	UPROPERTY(VisibleAnywhere, Category = "AZ")	int32 patrol_range_;
	UPROPERTY(VisibleAnywhere, Category = "AZ")	float patrol_delay_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") int32 percept_radius_;
};
