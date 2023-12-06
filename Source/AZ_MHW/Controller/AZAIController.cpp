#include "AZ_MHW/Controller/AZAIController.h"
#include <UObject/ConstructorHelpers.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AIPerceptionComponent.h>
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW\CharacterComponent\AZMonsterAggroComponent.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "Character/Player/AZPlayer.h"
#include "Character/Player/AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"

AAZAIController::AAZAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	sight_ = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	active_move_request_id_ = FAIRequestID::InvalidRequest;
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	//TODO move to OnPossess after bug fix
	sight_->SightRadius = 1500;
	sight_->LoseSightRadius = 2000;
	sight_->PeripheralVisionAngleDegrees = 90;
	sight_->SetMaxAge(3.0f);
	sight_->AutoSuccessRangeFromLastSeenLocation = 500;
	sight_->DetectionByAffiliation.bDetectEnemies = true;
	sight_->DetectionByAffiliation.bDetectFriendlies = false;
	sight_->DetectionByAffiliation.bDetectNeutrals = false;

	GetPerceptionComponent()->SetDominantSense(*sight_->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*sight_);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAZAIController::OnPlayerDetected);
}

void AAZAIController::OnPossess(APawn* const pawn) 
{
	Super::OnPossess(pawn);

	// Cast and save its owner
	owner_ = Cast<AAZMonster>(GetPawn());
	if (!owner_)
	{
		UE_LOG(AZMonster, Error, TEXT("[AZAIController] Non-AZMonster is possessing an AZAIController!"));
		return;
	}
	if (!owner_->IsAValidMonster())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZAIController] Possessing AZMonster id is not valid!"));
		return;
	}
	
	// Retrieve and set properties from the owner
	SetUpProperties();
	SetUpBehaviorTree();
	//SetUpPerceptionSystem();
	acceptance_radius_ = owner_->acceptance_radius_;
	
	if (IsValid(behavior_tree_))
	{
		UE_LOG(AZMonster, Log, TEXT("[AZAIController] Running a behavior tree"));
		RunBehaviorTree(behavior_tree_);
	}
	else
	{
		UE_LOG(AZMonster, Warning, TEXT("[AZAIController] Behavior tree is null"));
	}

	owner_->OnEnterCombat.AddDynamic(this, &AAZAIController::OnEnterCombat);
}

void AAZAIController::BeginPlay()
{
	Super::BeginPlay();
	if (!GetPerceptionComponent())
	{
		UE_LOG(AZMonster, Warning, TEXT("[AZAIController] Perception component is missing!"));
	}
}

void AAZAIController::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	// If active move request exists, check if reached goal
	if (owner_->IsMoving() && active_move_request_id_ != FAIRequestID::InvalidRequest)
	{
		if (GEngine)
		{
			float distance = FVector::Distance(owner_->GetActorLocation(), target_location_);
			GEngine->AddOnScreenDebugMessage(-1, delta_seconds, FColor::White, FString::Printf(TEXT("Remaining distance: %f"), distance));
		}
		if (HasReachedLocation(target_location_))
		{
			UE_LOG(AZMonster, Log, TEXT("[AZAIController] [ID %d] Goal location is reached"), (uint32)active_move_request_id_);
			const FAIMessage success_msg(UBrainComponent::AIMessage_MoveFinished, this, active_move_request_id_, FAIMessage::Success);
			FAIMessage::Send(this, success_msg);
			active_move_request_id_ = FAIRequestID::InvalidRequest;
		}
	}

	// DEBUG : if in combat, print distance to the target
	if (owner_->IsInCombat())
	{
		if (GEngine)
		{
			float distance = owner_->aggro_component_->GetDistance2DToTarget();
			GEngine->AddOnScreenDebugMessage(-1, delta_seconds, FColor::Black, FString::Printf(TEXT("Distance to target: %f"), distance));
		}
	}
}

void AAZAIController::SetUpPerceptionSystem()
{
	sight_->SightRadius = 1500;
	sight_->LoseSightRadius = 2000;
	sight_->PeripheralVisionAngleDegrees = 90;
	sight_->SetMaxAge(3.0f);
	sight_->AutoSuccessRangeFromLastSeenLocation = 500;
	sight_->DetectionByAffiliation.bDetectEnemies = true;
	sight_->DetectionByAffiliation.bDetectFriendlies = false;
	sight_->DetectionByAffiliation.bDetectNeutrals = false;

	GetPerceptionComponent()->SetDominantSense(*sight_->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*sight_);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAZAIController::OnPlayerDetected);

	//TODO Replace with this
	// sight_->SightRadius = sight_configs_.radius;
	// sight_->LoseSightRadius = sight_configs_.lose_radius;
	// sight_->PeripheralVisionAngleDegrees = sight_configs_.fov;
	// sight_->SetMaxAge(sight_configs_.max_age);
	// sight_->AutoSuccessRangeFromLastSeenLocation = sight_configs_.auto_success_range;
	// sight_->DetectionByAffiliation.bDetectEnemies = true;
	// sight_->DetectionByAffiliation.bDetectFriendlies = false;
	// sight_->DetectionByAffiliation.bDetectNeutrals = false;
}

//TEMP CHECK TOMORROW
ETeamAttitude::Type AAZAIController::GetTeamAttitudeTowards(const AActor& other_actor) const
{
	// Check if pawn
	const APawn* other_pawn = Cast<APawn>(&other_actor);
	if (!other_pawn) return ETeamAttitude::Neutral;

	// Check if AAZCharacter
	const AAZCharacter* other_character = Cast<AAZCharacter>(&other_actor);
	if (!other_character) return ETeamAttitude::Neutral;
	
	// Check if pawn implements GenericTeamAgentInterface
	const auto other_agent = Cast<IGenericTeamAgentInterface>(&other_actor);
	if (!other_agent) return ETeamAttitude::Neutral;

	// Check actor's team id
	if (owner_->GetBehaviorType() == EMonsterBehaviorType::Neutral)
	{
		return ETeamAttitude::Neutral;
	}
	else if (owner_->GetBehaviorType() == EMonsterBehaviorType::Friendly)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		// for now, simply return hostile attitude if the object type is different
		if (other_agent->GetGenericTeamId() != owner_->GetGenericTeamId()) return ETeamAttitude::Hostile;
		else return ETeamAttitude::Neutral;
	}
}

void AAZAIController::SetUpProperties()
{
	// Set up properties
	sight_configs_ = owner_->sight_configs_;
	patrol_range_ = owner_->patrol_range_;
	patrol_delay_ = owner_->patrol_delay_;
	percept_radius_ = owner_->percept_radius_;
}

void AAZAIController::SetUpBehaviorTree()
{
	// Get assets
	behavior_tree_ = UAZGameSingleton::instance()->monster_mgr_->GetBehaviorTree(owner_->name_);
	if (!IsValid(behavior_tree_)) return;
	
	// Initialise blackboard
	UBlackboardComponent* blackboard_component = Blackboard;
	UseBlackboard(behavior_tree_->BlackboardAsset, blackboard_component);
	SetBlackboardValueAsEnum(AZBlackboardKey::action_mode, UAZUtility::EnumToByte(EMonsterActionMode::Normal));
	SetBlackboardValueAsBool(AZBlackboardKey::is_triggered_by_sight, false);
}

FAIRequestID AAZAIController::GetNewMoveRequestID()
{
	active_move_request_id_ = next_request_id_++;
	return active_move_request_id_;
}

void AAZAIController::OnPlayerDetected(AActor* actor, FAIStimulus const stimulus) 
{
	if (owner_->IsInCombat()) return;
	if (Cast<AAZPlayer>(actor) && stimulus.WasSuccessfullySensed())
	{
		owner_->EnterCombat(actor, true);
	}
}

void AAZAIController::OnEnterCombat()
{
	// if active move request exists, abort it
	if (GetMoveRequestID() != FAIRequestID::InvalidRequest)
	{
		const FAIMessage fail_msg(UBrainComponent::AIMessage_MoveFinished, this, active_move_request_id_, FAIMessage::Failure);
		FAIMessage::Send(this, fail_msg);
		active_move_request_id_ = FAIRequestID::InvalidRequest;
	}
	
	AAZPlayer_Origin* target = owner_->aggro_component_->GetTargetRef();
	GetPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
	SetBlackboardValueAsObject(AZBlackboardKey::target_character, target);
}

void AAZAIController::SetBlackboardValueAsFloat(FName bb_key_name, float bb_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsFloat(bb_key_name, bb_value);
}

void AAZAIController::SetBlackboardValueAsBool(FName bb_key_name, bool bb_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsBool(bb_key_name, bb_value);
}

void AAZAIController::SetBlackboardValueAsEnum(FName bb_key_name, uint8 enum_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsEnum(bb_key_name, enum_value);
}

void AAZAIController::SetBlackboardValueAsObject(FName bb_key_name, UObject* bb_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsObject(bb_key_name, bb_value);
}

FPathFollowingRequestResult AAZAIController::MoveToLocation(const FVector& target_location)
{
	FPathFollowingRequestResult request_result;
	request_result.MoveId = GetNewMoveRequestID();
	target_location_ = target_location;
	
	if (HasReachedLocation(target_location_))
	{
		request_result.Code = EPathFollowingRequestResult::AlreadyAtGoal;
	}
	else if (owner_->IsMoving())
	{
		// If already moving, send failure message
		request_result.Code = EPathFollowingRequestResult::Failed;
	}
	else
	{
		// start move
		owner_->SetTargetAngle(owner_->GetRelativeAngleToLocation(target_location));
		owner_->SetMoveState(EMoveState::Walk);
		request_result.Code = EPathFollowingRequestResult::RequestSuccessful;		
	}
	
	return request_result;
}

bool AAZAIController::HasReachedLocation(const FVector& location) const
{
	const float distance_to_loc = FVector::Distance(owner_->GetActorLocation(), location);
	if (distance_to_loc <= acceptance_radius_) return true;
	else return false;
}
