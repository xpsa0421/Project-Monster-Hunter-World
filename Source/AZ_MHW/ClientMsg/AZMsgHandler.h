// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/ClientMsg/AZMsgObject.h"
#include "AZMsgHandler.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZMsgHandler : public UObject
{
	GENERATED_BODY()

public:
	UAZMsgHandler();
	virtual ~UAZMsgHandler();

	void Update(float deltaSeconds);
	void OnGameMsg(struct FAZGameMsg* newMsg);

	bool OnRegister(TScriptInterface<IAZMsgObject> obj);
	bool OnRemoveObject(TScriptInterface<IAZMsgObject> obj);
	
	void OnExecute(struct FAZGameMsg* new_msg);

public:
	UPROPERTY() TArray<TScriptInterface<IAZMsgObject>> msg_reciver;

private:
	TQueue<FAZGameMsg*> msg_queue;
};
