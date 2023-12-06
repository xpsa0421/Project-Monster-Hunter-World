// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayerController.h"
#include "InputActionValue.h"
#include "AZPlayerController_Server.generated.h"


struct FAZPlayerActionState;
class AAZPlayerState_Client;
class AAZPlayer_Origin;

/**
 * 서버용 클래스 인게임 맵에서 클라이언트의 인풋처리를 시뮬레이션 한다.
 * 클라이언트의 입력값을 서버 인게임 시뮬레이션의 캐릭터로 전달하는 역할입니다.
 * 또한 그러한 입력의 결과를 각 클라이언트의 원격 객체에 전파하는 역할도 있습니다. 
 */
UCLASS()
class AZ_MHW_API AAZPlayerController_Server : public AAZPlayerController
{
	GENERATED_BODY()

public:
	AAZPlayerController_Server();
#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	//virtual void Tick(float delta_seconds) override;
	/** */
	virtual void SetupInputComponent() override;
	/** */
	virtual void OnPossess(APawn* pawn) override;
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
public:
	void TempDevelopForceUpdatePlayer_Origin(int32 client_index, FVector pos);
#pragma region online function
	
public:
	/** int32 */
	UPROPERTY() TMap<int32, AAZPlayerState_Client*> online_player_character_state_;
	/** int32 */
	UPROPERTY() TMap<int32, AAZPlayer_Origin*> online_player_characters_;
	
#pragma region Receive Part(client->server)
	/** 캐릭터 선택시 선택한 캐릭터를 DB에서 가져오기*/
	void AddPlayerState_Origin(int32 client_index, int32 character_id);
	/** 캐릭터 데이터 제거하기*/
	void RemovePlayerState_Origin(int32 client_index);

	/** 원본을 추가해주세요..*/
	void AddPlayer_Origin(int32 client_index, int32 character_id);
	/** 원본을 제거해주세요. 서버는 인덱스 값을 가지고 있기 때문에 보낼게 없다.*/
	void RemovePlayer_Origin(int32 client_index);

	/** 원본을 움직여주세요. 서버는 인덱스값을 가지고 있기 때문에, 현재위치와 입력값을 전달*/
	void ActionPlayer_Origin(int32 client_index, const FVector& cur_pos, float cur_dir, float input_dir, int32 input_data);
	/** 원본의 장비를 바꿔주세요. 서버는 인덱스를 가지고 있기때문에 변경할 아이템만 보내면 된다.*/
	void EquipPlayer_Origin(int32 client_index, int32 item_id);
	/** 원본의 제스쳐를 실행해 주세요.*/
	void GesturePlayer_Origin(int32 client_index, int32 gesture_id);
#pragma endregion

#pragma region Send Part(server->client)
public:
	void BroadCast_AddPlayerState_Remotable(int32 client_index);
	void BroadCast_RemovePlayerState_Remotable(int32 client_index);
	
	void BroadCast_AddPlayer_Remotable(int32 client_index);
	void BroadCast_RemovePlayer_Remotable(int32 client_index);

	void BroadCast_ActionPlayer_Remotable(int32 client_index);
	void BroadCast_EquipPlayer_Remotable(int32 client_index, int32 item_id);
	void BroadCast_GesturePlayer_Remotable(int32 client_index, int32 gesture_id);
	
	void BroadCast_HitPlayer_Remotable(int32 client_index, float angle, int32 damage);
	void Send_HitPlayer_Playable(int32 client_index, float angle, int32 damage);
	void BroadCast_UpdatePlayerState_Remotable(int32 client_index, int32 capture_type, int32 capture_value, int32 capture_mask);
	void Send_UpdatePlayerState_Playable(int32 client_index, int32 capture_type, int32 capture_value, int32 capture_mask);
#pragma endregion
#pragma endregion 
};
