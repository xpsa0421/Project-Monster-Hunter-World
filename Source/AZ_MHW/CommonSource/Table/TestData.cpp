#include "AZ_MHW/CommonSource/Table/TestData.h"

void UTestData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test1");
	if (value_ptr != nullptr)
	{
		test1 = (*value_ptr).ToBool();
	}
	value_ptr = data.Find("test2");
	if (value_ptr != nullptr)
	{
		test2 = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test3");
	if (value_ptr != nullptr)
	{
		test3 = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test4");
	if (value_ptr != nullptr)
	{
		test4 = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test5");
	if (value_ptr != nullptr)
	{
		test5 = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test6");
	if (value_ptr != nullptr)
	{
		test6 = FCString::Atof(**value_ptr);
	}
	value_ptr = data.Find("test7");
	if (value_ptr != nullptr)
	{
		test7 = FCString::Atod(**value_ptr);
	}
	value_ptr = data.Find("test8");
	if (value_ptr != nullptr)
	{
		FDateTime::Parse(**value_ptr, test8);
	}
	value_ptr = data.Find("test9");
	if (value_ptr != nullptr)
	{
		test9 = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test10");
	if (value_ptr != nullptr)
	{
		test10 = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("test11");
	if (value_ptr != nullptr)
	{
		test11 = **value_ptr;
	}
	value_ptr = data.Find("test12");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			bool value;
			value = str.ToBool();
			test12.Add(value);
		}
	}
	value_ptr = data.Find("test13");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test13.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("test14");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test14.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("test15");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test15.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("test16");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test16.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("test17");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test17.Add(FCString::Atof(*str));
		}
	}
	value_ptr = data.Find("test18");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test18.Add(FCString::Atod(*str));
		}
	}
	value_ptr = data.Find("test19");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			FDateTime time;
			FDateTime::Parse(*str, time);
			test19.Add(time);
		}
	}
	value_ptr = data.Find("test20");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test20.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("test21");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test21.Add(FCString::Atoi(*str));
		}
	}
	value_ptr = data.Find("test22");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			test22.Add(str);
		}
	}
}
