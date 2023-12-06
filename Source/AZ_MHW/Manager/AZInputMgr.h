// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZInputMgr.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * InputMappingContext, InputAction 데이터들을 보유하는 매니저 클래스
 * BluePrint에서 사용해야 된다면, UCLASS에 지정자사용. 사용하기 쉽도록 TARRAY도 지원해야 한다. 
 */
UCLASS()
class AZ_MHW_API UAZInputMgr : public UObject
{
	GENERATED_BODY()
	
public:
	UAZInputMgr();
	void Init();
#pragma region Inherited function
protected:
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
public:
	/** !!따로 호출하지 마세오.(Controller SetupInputComponent에서 기본 호출)
	 * 
	 * 컨트롤러 제외한 기본 인풋매핑컨텍스트 사전등록 함수.
	 * 인풋액션바인딩 초기화구문이 있으니 컨트롤러의 액션바인딩보다 나중에 호출하지 마세요.
	 */
	void SetupDefaultBindAction(UInputComponent* player_input_component,const ULocalPlayer* local_player);
	/** */
	UFUNCTION() void AddInputMappingContext(const FName& name); 
	/** */
	UFUNCTION() void RemoveInputMappingContext(const FName& name);
	/** */
	UFUNCTION() void ClearInputMappingContext() const;
	/** */
	UFUNCTION() UInputAction* GetInputAction(const FName& name);
	/** */
	UFUNCTION() UInputMappingContext* GetInputMappingContext(const FName& name);
	/** */
	int32* GetInputMappingContextPriority(const FName& name);
	
private:
	/** 데이터에셋 맵(키: 접두사 제거 네임)*/
	UPROPERTY() TMap<FName, UInputMappingContext*> input_mapping_context_map_;
	/** 적용우선도 맵(키: 접두사 제거 네임)*/
	UPROPERTY() TMap<FName, int32> input_mapping_context_priority_map_;
	/** 인풋액션 맵(키: 접두사 제거 네임) */
	UPROPERTY() TMap<FName, UInputAction*> input_action_map_;

	UPROPERTY() const ULocalPlayer* local_player_;
	
	/** test용 함수 */
	void ActionTest();
};
