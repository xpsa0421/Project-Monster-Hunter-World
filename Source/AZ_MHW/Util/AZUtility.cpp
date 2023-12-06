// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Util/AZUtility.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"

DEFINE_LOG_CATEGORY(AZ_TEST)
DEFINE_LOG_CATEGORY(AZ_MOVE)
DEFINE_LOG_CATEGORY(AZ_ATTACK)
DEFINE_LOG_CATEGORY(AZ_DAMAGE)
DEFINE_LOG_CATEGORY(AZ_PLAYER)
DEFINE_LOG_CATEGORY(LogShipping)
DEFINE_LOG_CATEGORY(AZMonster)
DEFINE_LOG_CATEGORY(AZMonster_Network)
DEFINE_LOG_CATEGORY(AZMonster_Aggro)
DEFINE_LOG_CATEGORY(AZItem)
DEFINE_LOG_CATEGORY(AZ)

void UAZUtility::ShippingLog(const FString log_msg)
{
	UE_LOG(LogShipping, Log, TEXT("%s"), *log_msg);
}

float UAZUtility::MillisecondsToSeconds(const int32 milliseconds)
{
	return milliseconds / 1000.0f;
}

float UAZUtility::PerTenThousandToPerOne(const int32 per_ten_thousand)
{
	return per_ten_thousand / 10000.0f;
}

char* UAZUtility::FNameToCharArr(FName name)
{
	std::string std_str(TCHAR_TO_UTF8(*name.ToString()));
	char* char_arr = new char[std_str.length() + 1];
	strcpy_s(char_arr, std_str.length() + 1, std_str.c_str());
	return char_arr;
}

FName UAZUtility::CharArrToFName(const char* char_arr)
{
	return FName(UTF8_TO_TCHAR(char_arr));
}