// Copyright Epic Games, Inc. All Rights Reserved.

#include "SocketThreadGameMode.h"
#include "SocketThreadCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASocketThreadGameMode::ASocketThreadGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
