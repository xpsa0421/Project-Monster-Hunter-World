// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZGameMode.h"
#include "AZGameMode_Server.generated.h"

class UAZObjectMgr_Server;
class ULevelStreamingDynamic;

/**
 * 서버에서 필요한 방식으로 프로그램을 실행합니다.
 * 캐릭터, 컨트롤러 , HUD 등
 */
UCLASS()
class AZ_MHW_API AAZGameMode_Server : public AAZGameMode
{
	GENERATED_BODY()

public:
	AAZGameMode_Server();

#pragma region Inherited function
protected:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& map_name, const FString& options, FString& error_message) override;
	virtual void Tick(float delta_seconds) override;
#pragma endregion

public:
	// 디버깅용 blueprintReadWrite TEMP
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UAZObjectMgr_Server> object_mgr_;
	TObjectPtr<ULevelStreamingDynamic> combat_level_;
	
	//TQueue<> login_queue_;?
	//TQueue<> input_queue_;? 이런식으로 하는게 맞나...
	//client_index와 input값을 큐에 저장
};
