#pragma once

#include "LoginStruct.generated.h"

UENUM(BlueprintType)
enum class EForceKick : uint8
{
	NormalLogout, // 정상 로그아웃
	Max
};