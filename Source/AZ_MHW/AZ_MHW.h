// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
 
#include <CoreMinimal.h>

/** 이 게임 모듈의 헤더파일이 아닌 경우(= 엔진, 플러그인 헤더인 경우) < > 시스템경로 괄호로 헤더를 감싼다.*/
/** H파일의 경우 CoreMinimal.h가 제일 먼저 포함, 마지막은 자신의 .generated.h (UCLASS, USTRUCT의 경우)*/
/** H파일에서 자신의 상속클래스가 아니고 매개변수 형식(포인터가 아닌) 이외는 모두 전방선언*/

/** CPP파일의 경우 자신의 .h을 제일 먼저 포함후, 구현하기 위한 모든 헤더파일 포함*/

/** UObject파생클래스는 소멸자 사용하지 않도록 합니다. BeginDestroy 사용할 것*/


/*
일반적으로 거의 사용되는 헤더일람
#include <CoreMinimal.h>

#include <UObject/NoExportTypes.h>
#include <Templates/SubclassOf.h>


#include <Engine/GameInstance.h>//<-UObject

#include <GameFramework/Actor.h>//<-UObject
//#include <GameFramework/Info.h>//<-AActor
#include <GameFramework/GameModeBase.h>//<-AInfo
#include <GameFramework/GameMode.h>//<-AGameModeBase
#include <GameFramework/GameStateBase.h>//<-AInfo
#include <GameFramework/GameState.h>//<-AGameStateBase

#include <GameFramework/HUD.h>//<-Actor
#include <GameFramework/Controller.h>//<-AActor
#include <GameFramework/PlayerController.h>//<-AController

#include <GameFramework/PlayerState.h>//<-AInfo

#include <GameFramework/Pawn.h>//<-AActor
#include <GameFramework/Character.h>//<-APawn

#include <Camera/CameraActor.h>//<-AActor

//컴포넌트
#include <Components/>

//다양한 기본구현 함수
#include <Kismet/GameplayStatics.h>

//매크로
#include <UObject/ObjectMacros.h>
#include <UObject/ScriptsMacros.h>

*/
