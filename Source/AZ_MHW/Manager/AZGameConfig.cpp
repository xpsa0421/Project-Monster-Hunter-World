// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZGameConfig.h"

UAZGameConfig::UAZGameConfig()
{
    is_server_list_desplay = 0;
    default_server_index = 0;
}

void UAZGameConfig::Load()
{
    const TCHAR file_name[] = TEXT("DefaultAZConfig");
    FConfigCacheIni::LoadExternalIniFile(config_file_, file_name, TEXT(""), TEXT(""), false);
    if (config_file_.Num() == 0)
    {
        // 유저가 수동으로 카피해놓은 ini 파일이 없다며 ㄴ기본 경로를 읽는다.
        FConfigCacheIni::LoadLocalIniFile(config_file_, file_name, false);
    }

    FString str_temp;
    // [Server]
    {
        config_file_.GetBool(TEXT("Server"), TEXT("server_list_display"), is_server_list_desplay);
        config_file_.GetInt(TEXT("Server"), TEXT("default_server_index"), default_server_index);

        GConfig->SetFile(config_file_.Name.ToString(), &config_file_);

        TArray<FString> server_name_list;
        TArray<FString> server_ip_list;
        GConfig->GetArray(TEXT("Server"), TEXT("name"), server_name_list, file_name);
        GConfig->GetArray(TEXT("Server"), TEXT("ip"), server_ip_list, file_name);

        for (int index = 0; index < server_name_list.Num(); ++index)
        {
            if (index < server_ip_list.Num())
            {
                ServerInfo server_info;
                server_info.server_name = server_name_list[index];
                TArray<FString> temp_array;
                server_ip_list[index].ParseIntoArray(temp_array, TEXT(":"), true);
                server_info.server_ip = temp_array[0];
                server_info.server_port = FCString::Atoi(*temp_array[1]);
                array_server_info.Add(server_info);
            }
        }
    }

}

FString UAZGameConfig::GetDefaultServerIP()
{
    if (default_server_index >= array_server_info.Num())
    {
        return TEXT("Invalid default_server_index!");
    }
    return array_server_info[default_server_index].server_ip;
}

FString UAZGameConfig::GetDefaultServerName()
{
    if (default_server_index >= array_server_info.Num())
    {
        return TEXT("Invalid default_server_index!");
    }
    return array_server_info[default_server_index].server_name;
}

int32 UAZGameConfig::GetDefaultServerPort()
{
    if (default_server_index >= array_server_info.Num())
    {
        return 0;
    }
    return array_server_info[default_server_index].server_port;
}

void UAZGameConfig::ForEach_ServerList(std::function<void(FString server_ip, FString server_name)> func)
{
    for (const ServerInfo& server_info : array_server_info)
    {
        func(server_info.server_ip, server_info.server_name);
    }
}