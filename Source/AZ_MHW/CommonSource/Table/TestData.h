#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "TestData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UTestData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool test1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 test2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 test3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 test4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 test5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float test6;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) double test7;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FDateTime test8;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 test9;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 test10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString test11;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<bool> test12;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> test13;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> test14;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> test15;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> test16;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<float> test17;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<double> test18;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FDateTime> test19;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> test20;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<int32> test21;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> test22;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
