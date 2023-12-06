// Copyright Team AZ. All Rights Reserved.


#include "AZInputMgr.h"
#include <InputMappingContext.h>
#include <AssetRegistry/AssetRegistryModule.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

UAZInputMgr::UAZInputMgr()
{
	// 에셋 레지스트리 모듈을 얻음
	FAssetRegistryModule& asset_registry_module = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
#pragma region InputMappingContext_로드
	{
		const FString input_mapping_context_directory = TEXT("/Game/AZ/DataAsset/Input/InputMappingContext");
		
		// 디렉토리 내의 모든 에셋을 가져옴
		TArray<FAssetData> asset_data_list;
		asset_registry_module.Get().GetAssetsByPath(*input_mapping_context_directory, asset_data_list, true);

		// 에셋을 로드함
		for (const FAssetData& asset_data : asset_data_list)
		{
			// 에셋 경로로부터 에셋 이름을 추출함
			FString asset_name;
			FString path_part, extension_part;
			FPaths::Split(asset_data.GetSoftObjectPath().ToString(), path_part, asset_name,extension_part);
			
			// 에셋을 로드함
			if (UInputMappingContext* input_mapping_context = Cast<UInputMappingContext>(asset_data.GetAsset()))
			{
				// InputMappingContextMap에 추가함
				/** 에셋이름 제거(IA_)//상수로 4입력
				 * const FString AssetsPrefix = "IMC_";
				 * int32 PrefixSize = AssetsPrefix.Len();
				 * InputActionMap.Add(*AssetName.Mid(PrefixSize), InputAction);
				*/
				input_mapping_context_map_.Add(*asset_name.Mid(4), input_mapping_context);
				// InputMappingContextPriorityMap에 추가함
				/** 경로상의 상위 폴더를 숫자폴더로 만들어서 저장
				 * 경로상 숫자가 아니라면 자동 0으로 로드됨.
				 * InputMappingContext/InGame/0/IMC_XXXXXX;
				 * InputMappingContext/Lobby/0/IMC_XXXXXX;
				 * InputMappingContext/InGame/1/IMC_XXXXXX;
				 * 파일이름에 넣어도 사실 상관없다고는 생각하긴하는데..
				 */
				FString priority_part = path_part.Right(1);
				int32 priority = FCString::Atoi(*priority_part);
				input_mapping_context_priority_map_.Add(*asset_name.Mid(4), priority);
			}
		}
	}
#pragma endregion
#pragma region InputAction_로드
	{
		const FString input_action_directory = TEXT("/Game/AZ/DataAsset/Input/InputAction");
		
		// 디렉토리 내의 모든 에셋을 가져옴
		TArray<FAssetData> asset_data_list;
		asset_registry_module.Get().GetAssetsByPath(*input_action_directory, asset_data_list, true);

		// 에셋을 로드함
		for (const FAssetData& asset_data : asset_data_list)
		{
			// 에셋 경로로부터 에셋 이름을 추출함
			FString asset_name;
			FString path_part, extension_part;//안씀
			FPaths::Split(asset_data.GetSoftObjectPath().ToString(), path_part, asset_name,extension_part);
		
			// 에셋을 로드함
			if (UInputAction* input_action_part = Cast<UInputAction>(asset_data.GetAsset()))
			{
				// InputActionMap에 추가함
				/*
				 *에셋이름 제거(IA_)//상수로 3입력
				const FString AssetsPrefix = "IA_";
				int32 PrefixSize =AssetsPrefix.Len();
				InputActionMap.Add(*AssetName.Mid(PrefixSize), InputAction);
				*/
				input_action_map_.Add(*asset_name.Mid(3), input_action_part);
			}
		}
	}
	//사용한 에셋 레지스트리 모듈 해제
	//(다른곳에서도 이것을 사용하고 있는데 해제한다면? 애초에 계속 로드해놓을 필요가 없는 모듈.)
	FModuleManager::Get().UnloadModule(TEXT("AssetRegistry"));
#pragma endregion
}

void UAZInputMgr::Init()
{
}

void UAZInputMgr::BeginDestroy()
{
	Super::BeginDestroy();
	input_mapping_context_priority_map_.Empty();
	input_mapping_context_map_.Empty();
	input_action_map_.Empty();
}

void UAZInputMgr::SetupDefaultBindAction(UInputComponent* player_input_component, const ULocalPlayer* local_player)
{
	//클라에서 마지막을 등록된 플레이어
	local_player_ = local_player;
	ClearInputMappingContext();
	if (UEnhancedInputComponent* enhanced_input_component = CastChecked<UEnhancedInputComponent>(player_input_component))
	{
		enhanced_input_component->ClearActionBindings();
		
		// if(const auto& input_action_asset = GetInputAction("InputActionName"))
		// {
		// 		enhanced_input_component->BindAction(input_action_asset, ETriggerEvent::Triggered,this, &UAZInputMgr::ActionTest);
		// }
	}
}

UInputMappingContext* UAZInputMgr::GetInputMappingContext(const FName& name)
{
	if(const auto input_mapping_context_asset = input_mapping_context_map_.Find(name))
	{
		return *input_mapping_context_asset;
	}
	return nullptr;
}
int32* UAZInputMgr::GetInputMappingContextPriority(const FName& name)
{
	if(const auto input_mapping_context_priority = input_mapping_context_priority_map_.Find(name))
	{
		return input_mapping_context_priority;
	}
	return nullptr;
}
UInputAction* UAZInputMgr::GetInputAction(const FName& name)
{
	if(const auto input_action_asset = input_action_map_.Find(name))
	{
		return *input_action_asset;
	}
	return nullptr;
}

void UAZInputMgr::AddInputMappingContext(const FName& name)
{
	if(local_player_ == nullptr) return;
	if (UEnhancedInputLocalPlayerSubsystem* enhanced_input_local_player_subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(local_player_))
	{
		if(const auto input_mapping_context = GetInputMappingContext(name))
		{
			if(const auto context_priority = GetInputMappingContextPriority(name))
			{
				enhanced_input_local_player_subsystem->AddMappingContext(input_mapping_context, *context_priority);
			}
		}
		else
		{
			
			//인풋매핑컨텍스트 없음
		}
	}
	else
	{
	}
}
void UAZInputMgr::RemoveInputMappingContext(const FName& name)
{
	if(local_player_ == nullptr) return;
	if (UEnhancedInputLocalPlayerSubsystem* enhanced_input_local_player_subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(local_player_))
	{
		if(const auto input_mapping_context = GetInputMappingContext(name))
		{
			enhanced_input_local_player_subsystem->RemoveMappingContext(input_mapping_context);
		}
		else
		{
			//인풋매핑컨텍스트 없음
		}
	}
	else
	{
	}
}
void UAZInputMgr::ClearInputMappingContext() const
{
	if(local_player_ == nullptr) return;
	if (UEnhancedInputLocalPlayerSubsystem* enhanced_input_local_player_subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(local_player_))
	{
		enhanced_input_local_player_subsystem ->ClearAllMappings();
	}
	else
	{
	}
}

void UAZInputMgr::ActionTest()
{
}