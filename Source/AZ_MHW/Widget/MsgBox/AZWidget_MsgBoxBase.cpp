// Copyright Team AZ. All Rights Reserved.


#include "Widget/MsgBox/AZWidget_MsgBoxBase.h"
#include "Components/TextBlock.h"

void UAZWidget_MsgBoxBase::Init()
{
	Super::Init();
}

void UAZWidget_MsgBoxBase::SetTitle(const FString& titleStr)
{
	if (c_text_title_)
	{
		c_text_title_->SetText(FText::FromString(titleStr));
	}
}

void UAZWidget_MsgBoxBase::SetDesc(const FString& msg)
{
	if (c_text_message_)
	{
		c_text_message_->SetText(FText::FromString(msg));
	}
}