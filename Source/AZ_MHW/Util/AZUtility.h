// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "type_traits"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum.hpp"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum_switch.hpp"
#include "AZUtility.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AZ_TEST, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ_MOVE, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ_ATTACK, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ_DAMAGE, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ_PLAYER, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(LogShipping, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZMonster, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZMonster_Network, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZMonster_Aggro, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZItem, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ, Log, All)

#define PRINT_LOG(str)		UE_LOG(AZ_TEST, Warning, TEXT(str))
#define PRINT_FUNCTION()	UE_LOG(AZ_TEST, Warning, TEXT("%s"), __FUNCTIONW__)

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZUtility : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Utility") static void ShippingLog(const FString log_msg);
	template<typename EType>
	static FString EnumToString(EType type)
	{
		static_assert(std::is_enum_v<EType>, "Non-Enum class is being used");
		auto type_str = magic_enum::enum_name(type);
		FString test(type_str.data());
		return test;
	}
	template<typename EType>
	static EType StringToEnum(const FString& string)
	{
		// Returns the enum value of the string or the first enum element if unavailable
		static_assert(std::is_enum_v<EType>, "Non-Enum class is being used");
		std::string convert_type = TCHAR_TO_ANSI(*string);
		std::optional<EType> type = magic_enum::enum_cast<EType>(convert_type);
		return *type;
	}
	template<typename EType>
	static TArray<EType> StringArrToEnumArr(const TArray<FString>& string_arr)
	{
		static_assert(std::is_enum_v<EType>, "Non-Enum class is being used");
		TArray<EType> enum_arr;
		for (auto string : string_arr)
		{
			enum_arr.Add(StringToEnum<EType>(string));
		}
		return enum_arr;
	}
	template<typename EType>
	static EType StringArrToBitMaskEnum(TArray<FString> enum_string_arr)
	{
		// Converts an array of enum strings into a bit masked enum type
		static_assert(std::is_enum_v<EType>, "Non-Enum class is being used");
		auto enum_value = magic_enum::enum_value<EType>(0);
		
		if (!(enum_string_arr[0] == "NULL" || enum_string_arr[0] == "None"))
		{
			for (const auto enum_str : enum_string_arr)
			{
				enum_value |= UAZUtility::StringToEnum<EType>(enum_str);
			}
		}
		return enum_value;
	}
	template<typename EType>
	static uint8 EnumToByte(EType enum_type)
	{
		return static_cast<uint8>(enum_type);
	}
	
	static float MillisecondsToSeconds(const int32 milliseconds);
	static float PerTenThousandToPerOne(const int32 per_ten_thousand);

	// for server-client transmission
	static char* FNameToCharArr(FName name);
	static FName CharArrToFName(const char* char_array);
};
