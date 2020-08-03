// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerControlGameMode.h"
#include "PlayerControlCharacter.h"
#include "UObject/ConstructorHelpers.h"

APlayerControlGameMode::APlayerControlGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
