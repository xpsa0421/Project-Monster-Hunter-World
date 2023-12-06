// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "AZTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZTextBlock : public UTextBlock
{
	GENERATED_BODY()
	
public:
	UAZTextBlock();
	void SetTextFromString(const FString& text);

public:
	virtual void SetText(FText InText) override;
	virtual void SynchronizeProperties() override;

	int32 GetTextTableID() { return init_text_table_id_; }
	void SetTextTableID(int32 idx);

	ETextJustify::Type GetJustification() const { return Justification; }

public:
	UPROPERTY(EditAnywhere, Category = "AZTextBlock")
		int32 init_text_table_id_;	// 초기화 될때 사용되는 텍스트 테이블 ID
};
