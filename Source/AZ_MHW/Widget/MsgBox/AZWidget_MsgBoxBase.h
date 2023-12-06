// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_MsgBoxBase.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_MsgBoxBase : public UAZWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void SetDesc(const FString& msg);
	virtual void SetTitle(const FString& titleStr);

	EUIMsgBoxType GetMsgBoxType() { return msg_box_type_; }
	void SetMsgBoxType(EUIMsgBoxType msg_box_type) { msg_box_type_ = msg_box_type; }

protected:
	EUIMsgBoxType msg_box_type_;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* c_text_title_;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* c_text_message_;
};
