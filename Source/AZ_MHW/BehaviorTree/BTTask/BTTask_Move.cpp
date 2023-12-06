// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/BehaviorTree/BTTask/BTTask_Move.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Util/AZUtility.h"

//@TODO Change to rootmotion based movement

UBTTask_Move::UBTTask_Move()
{
	NodeName = "Move";
	final_dest_ = FVector::ZeroVector;
	move_request_id_ = FAIRequestID::InvalidRequest;
	bNotifyTaskFinished = true;
}

void UBTTask_Move::OnMessage(UBehaviorTreeComponent& owner_comp, uint8* node_memory, FName message, int32 sender_id, bool is_success)
{
	if (message != UBrainComponent::AIMessage_MoveFinished) return;
	const EBTTaskStatus::Type Status = owner_comp.GetTaskStatus(this);
	owner_->SetMoveState(EMoveState::StopMove);
	
	if (Status == EBTTaskStatus::Aborting)
	{
		UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move][%d/%d] Received Move Abort Message"), path_index_, path_points_.Num() - 1);
		FinishLatentAbort(owner_comp);
		return;
	}

	if (Status == EBTTaskStatus::Active)
	{
		UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move][%d/%d] Received Move Success Message"), path_index_, path_points_.Num() - 1);
		if (!is_success)
		{
			FinishLatentTask(owner_comp, EBTNodeResult::Failed);
		}
		else if (path_index_ < path_points_.Num() - 1)
		{
			path_index_++;
			MoveBegin(owner_comp);
		}
		else
		{
			UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] Last move ended"));
			FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
		}
	}
}

void UBTTask_Move::OnTaskFinished(UBehaviorTreeComponent& owner_comp, uint8* node_memory, EBTNodeResult::Type task_result)
{
	Super::OnTaskFinished(owner_comp, node_memory, task_result);
	UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move] Task Finished"));
}

EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	path_index_ = 1;

	Super::ExecuteTask(owner_comp, node_memory);
	owner_ = Cast<AAZMonster>(owner_comp.GetAIOwner()->GetPawn());
	FVector curr_location = owner_->GetActorLocation();
	final_dest_ = owner_comp.GetBlackboardComponent()->GetValueAsVector(AZBlackboardKey::target_location);
	bool is_move_start = false;  
	
	if ((curr_location != final_dest_))// && (owner_->server_movement_component_->move_type_ != next_move_type))
	{
		is_move_start = true;
	} 
	else 
	{
		// TODO: when monster needs to move but is stunned/debuffed
	}
	
	if (is_move_start)
	{
		UNavigationPath* nav_path;
		UNavigationSystemV1* nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		nav_path = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), curr_location, final_dest_, owner_.Get());

		if (!nav_path)
		{
			UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move] Request failed"));
			return EBTNodeResult::Failed;
		}
		else if (nav_path->PathPoints.IsEmpty())
		{
			UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move] No valid path found"));
			return EBTNodeResult::Failed;
		}
		else
		{
			path_points_.Empty();
			path_points_ = nav_path->PathPoints;
			
			UE_LOG(AZMonster, Warning, TEXT("---------------------------------------------------"));
			UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] Start: %s | End: %s"), *(owner_->GetActorLocation().ToString()), *(final_dest_.ToString()));
			for (int point_iter = 1; point_iter < nav_path->PathPoints.Num(); point_iter++)
			{
#if WITH_EDITOR
				UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] [PathPoint %d] %s"), point_iter, *(nav_path->PathPoints[point_iter].ToString()));
				//DrawDebugSphere(GetWorld(), nav_path->PathPoints[point_iter], 5.0f, 16, FColor::Red, false, 10.0f, 0U, 20.f);
				//DrawDebugLine(GetWorld(), nav_path->PathPoints[point_iter - 1], nav_path->PathPoints[point_iter], FColor::Red, false, 10.0f, 0U, 5.f);
#endif
			}
		}

		EBTNodeResult::Type move_result = MoveBegin(owner_comp);
		if (move_result != EBTNodeResult::InProgress)
		{
			UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move] Move finishing with result %s"), *UEnum::GetValueAsString(move_result));
			FinishLatentTask(owner_comp, move_result);
		}
	}
	
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Move::MoveBegin(UBehaviorTreeComponent& owner_comp)
{
	EBTNodeResult::Type move_result = EBTNodeResult::Failed;
	if (path_index_ >= path_points_.Num())
	{
		UE_LOG(AZMonster, Error, TEXT("[UBTTask_Move] Accessing Invalid Index: %d"), path_index_);
		return move_result;
	}

	// Request movement
	FPathFollowingRequestResult move_req_result = owner_->GetController()->MoveToLocation(path_points_[path_index_]);
	UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] [ID%s] Requested move to pathpoint#%d"), *(move_req_result.MoveId.ToString()), path_index_);

	// Check request result
	if (move_req_result.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		// If the request is successful, wait for movement to end
		UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] [ID%s] Waiting for move to pathpoint #%d to finish"), *(move_req_result.MoveId.ToString()), path_index_);
		WaitForMessage(owner_comp, UBrainComponent::AIMessage_MoveFinished, move_req_result.MoveId);
		move_result = EBTNodeResult::InProgress;
	}
	else if (move_req_result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		// If already at goal, move to next path point
		UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] [ID%s] Pathpoint #%d already arrived"), *(move_req_result.MoveId.ToString()), path_index_);

		if (path_index_ >= path_points_.Num() - 1)
		{
			move_result = EBTNodeResult::Succeeded;
		}
		else
		{
			path_index_++;
			move_result = MoveBegin(owner_comp);
		}
	}
	else
	{
		UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move] [ID%s] Request failed"), *(move_req_result.MoveId.ToString()));
		move_result = EBTNodeResult::Failed;
	}
	
	return move_result;
}










//-------------------------------------------------------------------------
//	CURRENTLY NOT IN USE !!!
//	MOVEMENT USING FORCE ROOT LOCK + Built in MoveTo Function
//-------------------------------------------------------------------------
// EBTNodeResult::Type UBTTask_Move::MoveBegin(UBehaviorTreeComponent& owner_comp)
// {
// 	if (path_index_ >= path_points_.Num())
// 	{
// 		UE_LOG(AZMonster, Error, TEXT("[UBTTask_Move] Accessing Invalid Index: %d"), path_index_);
// 		return EBTNodeResult::Failed;
// 	}
//
// 	EBTNodeResult::Type move_result = EBTNodeResult::Failed;
// 	AAZAIController* controller = Cast<AAZAIController>(owner_->GetController());
// 	FPathFollowingRequestResult move_req_result;
//
// 	FAIMoveRequest move_req;
// 	move_req.SetNavigationFilter(*controller->GetDefaultNavigationFilterClass());
// 	move_req.SetAllowPartialPath(true);
// 	move_req.SetAcceptanceRadius(50.0f);
// 	move_req.SetCanStrafe(false);
// 	move_req.SetReachTestIncludesAgentRadius(true);
// 	move_req.SetReachTestIncludesGoalRadius(false);
// 	move_req.SetProjectGoalLocation(true);
// 	move_req.SetUsePathfinding(true);
// 	move_req.SetGoalLocation(path_points_[path_index_]);
//
// 	if (!move_req.IsValid())
// 	{
// 		UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move] Invalid move request"));
// 		move_result = EBTNodeResult::Failed;
// 	}
// 	else
// 	{
// 		move_req_result = controller->MoveTo(move_req);
// 		UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] [ID%s] Beginning move to pathpoint#%d"), *(move_req_result.MoveId.ToString()), path_index_);
//
// 		if (move_req_result.Code == EPathFollowingRequestResult::RequestSuccessful)
// 		{
// 			owner_->SetTargetAngle(owner_->GetRelativeAngleToLocation(path_points_[path_index_]));
// 			owner_->SetMoveState(EMoveState::Walk);
//
// 			UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] [ID%s] Waiting for move to pathpoint #%d to finish"), *(move_req_result.MoveId.ToString()), path_index_);
// 			WaitForMessage(owner_comp, UBrainComponent::AIMessage_MoveFinished, move_req_result.MoveId);
// 			move_result = EBTNodeResult::InProgress;
// 		}
// 		else if (move_req_result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
// 		{
// 			UE_LOG(AZMonster, Log, TEXT("[UBTTask_Move] [ID%s] Pathpoint #%d already arrived"), *(move_req_result.MoveId.ToString()), path_index_);
//
// 			if (path_index_ >= path_points_.Num() - 1)
// 			{
// 				move_result = EBTNodeResult::Succeeded;
// 			}
// 			else
// 			{
// 				path_index_++;
// 				move_result = MoveBegin(owner_comp);
// 			}
// 		}
// 		else
// 		{
// 			UE_LOG(AZMonster, Warning, TEXT("[UBTTask_Move] [ID%s] Request failed"), *(move_req_result.MoveId.ToString()));
// 			move_result = EBTNodeResult::Failed;
// 		}
// 	}
//
// 	return move_result;
// }


//-------------------------------------------------------------------------
//	CURRENTLY NOT IN USE !!!
//	MOVEMENT PER FRAME
//-------------------------------------------------------------------------

//	//owner_ = Cast<AAZMonster>(owner_comp.GetAIOwner()->GetPawn());
//	//if (!owner_->server_movement_component_->IsMovable())
//	//{
//	//	return EBTNodeResult::Failed;
//	//}
//
//	//EMoveType next_move_type = EMoveType(owner_comp.GetBlackboardComponent()->GetValueAsEnum(AZBlackboardKey::move_type));
//	//FVector next_pos = owner_comp.GetBlackboardComponent()->GetValueAsVector(AZBlackboardKey::target_location);
//	//bool is_move_start = false;
//
//	//if ((owner_->GetActorLocation() != next_pos) && (owner_->server_movement_component_->move_type_ != next_move_type))
//	//{
//	//	is_move_start = true;
//	//}
//	//else 
//	//{
//	//	// TODO: �� �����ϴµ� ���� ������ �� ���� ���
//	//}
//
//	//if (is_move_start)
//	//{
//	//	result = MoveBegin(owner_comp, next_move_type, next_pos, 0);
//	//}
//
//	//return result;
////}
//
//
//EBTNodeResult::Type UBTTask_Move::MoveBegin(UBehaviorTreeComponent& owner_comp, EMoveType next_move_type, FVector dest_pos, double angle)
//{
//	//cur_pos_ = owner_->GetActorLocation();
//	//UE_LOG(AZ, Log, TEXT("[Start Pos] %s [Dest Pos] %s"), *(cur_pos_.ToString()), *(dest_pos.ToString()));
//
//	//if (!owner_->server_movement_component_->IsMovable())
//	//{
//	//	return EBTNodeResult::Failed;
//	//}
//
//	//UNavigationPath* nav_path;
//	//UNavigationSystemV1* nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
//
//	//if (!owner_)
//	//{
//	//	UE_LOG(AZ, Log, TEXT("[UBTTask_Move] [MoveBegin] Owner not set."));
//	//	return EBTNodeResult::Failed;
//	//}
//
//	//nav_path = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), cur_pos_, dest_pos, owner_);
//	//if (!nav_path)
//	//{
//	//	// send stop packet
//	//	UE_LOG(AZ, Log, TEXT("[UBTTask_Move] Path not found."));
//	//	return EBTNodeResult::Failed;
//	//}
//	//else
//	//{
//	//	path_points_.Empty();
//	//	path_points_ = nav_path->PathPoints;
//
//	//	for (int point_iter = 1; point_iter < nav_path->PathPoints.Num(); point_iter++)
//	//	{
//	//		UE_LOG(AZ, Log, TEXT("[PathPoint %d] %s"), point_iter, *(nav_path->PathPoints[point_iter].ToString()));
//	//		DrawDebugSphere(GetWorld(), nav_path->PathPoints[point_iter], 5.0f, 16, FColor::Red, false, 4.0f, 0U, 20.f);
//	//		DrawDebugLine(GetWorld(), nav_path->PathPoints[point_iter - 1], nav_path->PathPoints[point_iter], FColor::Red, false, 4.0f, 0U, 5.f);
//	//	}
//
//	//	EBTNodeResult::Type move_result = MoveLoop(owner_comp);
//	//	UE_LOG(AZ, Log, TEXT("------------------------------------------------"));
//	//	// TODO: Set State
//	//	// TODO: Send move packet ??
//
//		return EBTNodeResult::Succeeded;//move_result;
//	//}
//}
//
//EBTNodeResult::Type UBTTask_Move::MoveLoop(UBehaviorTreeComponent& owner_comp)
//{
//	//if (!owner_)
//	//{
//	//	UE_LOG(AZ, Log, TEXT("OWNER NOT SET"));
//	//	return EBTNodeResult::Failed;
//	//}
//
//
//	//while (!path_points_.IsEmpty())
//	//{
//	//	float distance_per_frame = GetWorld()->GetDeltaSeconds() * owner_->server_movement_component_->GetMoveSpeed();
//
//	//	while (distance_per_frame > 0)
//	//	{
//	//		FVector next_pos = path_points_[0];
//	//		double distance_to_next_pos = FVector::Distance(cur_pos_, next_pos);
//
//	//		// if remaining distance to the next position is smaller than 
//	//		// the distance that can be moved per frame, move directly to the next position at once
//	//		if (distance_to_next_pos <= distance_per_frame)
//	//		{
//	//			//UE_LOG(AZ, Log, TEXT("[FROM] %s [TO] %s"), *(cur_pos_.ToString()), *(next_pos.ToString()));
//	//			//UE_LOG(AZ, Log, TEXT("Checkpoint reached"));
//	//			cur_pos_ = next_pos;
//	//			path_points_.RemoveAt(0);
//	//			distance_per_frame -= distance_to_next_pos;
//
//	//			if (!path_points_.IsEmpty())
//	//			{
//	//				// TODO: Calculate angle
//	//				if (owner_->server_movement_component_->IsMovable())
//	//				{
//	//					// TODO: Send Packet Move
//	//					//AAZAIController* controller = Cast<AAZAIController>(owner_->GetController());
//	//					//controller->MoveToLocation(path_points_)
//	//				}
//	//			}
//	//			else
//	//			{
//	//				// TODO: Make it a function
//	//				owner_->server_movement_component_->move_type_ = EMoveType::Stop;
//	//				owner_->move_type_ = EMoveType::Stop;
//	//				//AAZAIController* controller = Cast<AAZAIController>(owner_->GetController());
//	//				//controller->MoveToLocation(cur_pos_);
//
//	//				break;
//	//			}
//	//		}
//	//		else
//	//		{
//	//			// TODO: Make it a function
//	//			FVector direction = next_pos - cur_pos_;
//	//			direction.Normalize();
//	//			next_pos = cur_pos_ + direction * distance_per_frame;
//	//			//UE_LOG(AZ, Log, TEXT("[FROM] %s [TO] %s"), *(cur_pos_.ToString()), *(next_pos.ToString()));
//
//	//			cur_pos_ = next_pos;
//	//			distance_per_frame = 0;
//	//		}
//	//	}
//	//	// TODO: Calculate angle
//	//	AAZAIController* controller = Cast<AAZAIController>(owner_->GetController());
//
//	//	if (!controller)
//	//	{
//	//		UE_LOG(AZ, Log, TEXT("CONTROLLER NOT SET"));
//	//		return EBTNodeResult::Failed;
//	//	}
//
//	//	FAIMoveRequest move_req;
//	//	move_req.SetNavigationFilter(*controller->GetDefaultNavigationFilterClass());
//	//	move_req.SetAllowPartialPath(true);
//	//	move_req.SetAcceptanceRadius(5.0f);
//	//	move_req.SetCanStrafe(false);
//	//	move_req.SetReachTestIncludesAgentRadius(true);
//	//	move_req.SetReachTestIncludesGoalRadius(true);
//	//	move_req.SetProjectGoalLocation(true);
//	//	move_req.SetUsePathfinding(true);
//	//	move_req.SetGoalLocation(cur_pos_);
//
//	//	if (move_req.IsValid())
//	//	{
//	//		FPathFollowingRequestResult move_req_result = controller->MoveTo(move_req);
//
//	//		if (move_req_result.Code == EPathFollowingRequestResult::RequestSuccessful)
//	//		{
//	//			//MyMemory->MoveRequestID = RequestResult.MoveId;
//	//			//WaitForMessage(owner_comp, UBrainComponent::AIMessage_MoveFinished, move_req_result.MoveId);
//	//			//return EBTNodeResult::InProgress;
//	//		}
//	//		else if (move_req_result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
//	//		{
//	//			//UE_LOG(AZ, Log, TEXT("Already At Goal"));
//	//			//return EBTNodeResult::Succeeded;
//	//		}
//	//	}
//	//}
//
//	return EBTNodeResult::Succeeded;
//}
//
