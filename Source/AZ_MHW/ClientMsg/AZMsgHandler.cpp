// Fill out your copyright notice in the Description page of Project Settings.

#include "AZ_MHW/ClientMsg/AZMsgHandler.h"
#include "AZ_MHW/CommonSource/AZGameMsg.h"
#include "AZ_MHW/CommonSource/Define/GameDefine.h"
#undef GetObjectW
#undef GetObject
UAZMsgHandler::UAZMsgHandler()
{

}

UAZMsgHandler::~UAZMsgHandler()
{
	FAZGameMsg* e = nullptr;
	if (msg_queue.Dequeue(e))
	{
		SAFE_DELETE(e);
	}
	msg_queue.Empty();
}

void UAZMsgHandler::Update(float deltaSeconds)
{
	FAZGameMsg* msg;
	if (msg_queue.Dequeue(msg))
	{
		OnExecute(msg);

		SAFE_DELETE(msg);
		msg = nullptr;
	}
}

void UAZMsgHandler::OnGameMsg(struct FAZGameMsg* newMsg)
{
	if (newMsg->bIsQueueMsg)
	{
		check(newMsg);
		msg_queue.Enqueue(new FAZGameMsg(*newMsg));
	}
	else
	{
		OnExecute(newMsg);
	}
}

bool UAZMsgHandler::OnRegister(TScriptInterface<IAZMsgObject> obj)
{
	int32 index = msg_reciver.Add(obj);
	if (index != INDEX_NONE)
		return true;
	return false;
}

bool UAZMsgHandler::OnRemoveObject(TScriptInterface<IAZMsgObject> obj)
{
	msg_reciver.RemoveSingleSwap(obj);
	return true;
}

void UAZMsgHandler::OnExecute(struct FAZGameMsg* new_msg)
{
	for (int32 i = 0; i < msg_reciver.Num(); ++i)
	{
		const auto& msgObject = msg_reciver[i];

		msgObject->OnGameMsg(new_msg);

		if (!IsValid(msgObject.GetObject()))
		{
			msg_reciver.RemoveAt(i--);
			continue;
		}

	}
}

