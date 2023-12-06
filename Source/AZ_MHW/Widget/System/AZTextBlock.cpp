// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/System/AZTextBlock.h"

#include "AZ_MHW/Manager/AZTableMgr.h"

#define LOCTEXT_NAMESPACE "UMG"

UAZTextBlock::UAZTextBlock()
{
}

void UAZTextBlock::SetTextFromString(const FString& text)
{
	SetText(FText::FromString(text));
}

void UAZTextBlock::SetText(FText InText)
{
	Super::SetText(InText);
}

void UAZTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	//TODO 텍스트 테이블 필요시 넣기
	/*
	if (init_text_table_id_ != 0 && GetGameSingleton()->TableMgr)
		SetText(FText::FromString(*AZ_GetString(init_text_table_id_)));
		*/
}

void UAZTextBlock::SetTextTableID(int32 idx)
{
	// TODO 텍스트 테이블 필요시 넣기
	/*
	if (idx != 0 && GetGameSingleton()->table_mgr)
	{
		init_text_table_id_ = idx;
		SetText(FText::FromString("test"));
	}
	*/
}

/*const FText UAZTextBlock::GetPaletteCategory()
{
	return FText();
}*/
