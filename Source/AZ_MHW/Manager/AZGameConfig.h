// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZGameConfig.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameConfig : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY() bool is_server_list_desplay;
	UPROPERTY() int32 default_server_index;

private:
	FConfigFile config_file_;

	struct ServerInfo
	{
		FString server_name;
		FString server_ip;
		int32 server_port;
	};
	TArray<ServerInfo> array_server_info;

public:
	UAZGameConfig();

	void Load();

	FString GetDefaultServerIP();
	FString GetDefaultServerName();
	int32 GetDefaultServerPort();
	void ForEach_ServerList(std::function<void(FString server_ip, FString server_name)> func);
};
