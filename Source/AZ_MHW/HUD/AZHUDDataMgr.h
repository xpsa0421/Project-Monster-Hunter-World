// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/HUD/AZHUDDefine.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZHUDDataMgr.generated.h"

UENUM()
enum class EAZWidgetDataRequestState
{
	None,
	Requested,
	Completed
};

USTRUCT(BlueprintType)
struct FAZWidgetData
{
	GENERATED_USTRUCT_BODY()

public:
	class UAZWidget* GetWidget();
	class UAZWidget* GetOrCreateWidget(bool& bGetWidget, UAZGameInstance* game_instance);
	bool IsWidgetValid() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZWidgetData")
	EUILayer layer = EUILayer::Scene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZWidgetData")
	bool is_stay_in_viewport = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZWidgetData")
	FString widget_full_path;

	uint16 widget_id;
	bool disable_world_rendering = false;

protected:
	UPROPERTY()
	TWeakObjectPtr<UAZWidget> widget_ptr;
};

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZHUDDataMgr : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();
	void Reset();
	FAZWidgetData* GetWidgetData(EUIName ui_name_enum);
	const FAZWidgetData* GetWidgetData(EUIName ui_name_enum) const;
	FAZWidgetData* GetSubWidgetData(ESubUIName ui_name_enum);
	TMap<EUIName, FAZWidgetData>& GetWidgetDatas() { return widget_datas; }

	EAZWidgetDataRequestState GetWidgetDataRequestState(EUIName ui_name);
	bool RequestWidgetData(EUIName ui_name);
	void OnSuccessWidgetDataRequest(EUIName ui_name);
	void OnFailedWidgetDataRequest(EUIName ui_name);

private:
	void InitWidgetData(EUILayer ui_layer, EUIName ui_name, bool is_stay_in_viewport, bool disable_world_rendering, FString widget_path);
	void InitWidgetDataRequestState(EUIName ui_name);

	TMap<EUIName, FAZWidgetData> widget_datas;
	TMap<ESubUIName, FAZWidgetData> sub_widget_datas;
	TMap<EUIName, EAZWidgetDataRequestState> widget_data_request_states;

protected:
	UPROPERTY() class UAZGameInstance* game_instance_;
};
