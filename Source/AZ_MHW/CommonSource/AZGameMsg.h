// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum class EGameMsgType : uint8
{
	None,
	DestroyedObject
};

struct FAZGameMsg
{
	EGameMsgType MsgType;
	bool bIsQueueMsg;

	FAZGameMsg()
	{

	}
	FAZGameMsg(EGameMsgType msgType)
		: MsgType(msgType)
	{
		bIsQueueMsg = false;
	}
	FAZGameMsg(const FAZGameMsg& rhs)
	{
		*this = rhs;
	}
	virtual ~FAZGameMsg() {}
};

struct FDestroyedObject : public FAZGameMsg
{
	int32 object_serial;
	TWeakObjectPtr<AActor> object_ptr;
	FDestroyedObject() : FAZGameMsg(EGameMsgType::DestroyedObject) {}
	FDestroyedObject(const int32 serial) : FDestroyedObject()
	{
		object_serial = serial;
	}
};

