// Copyright Epic Games, Inc. All Rights Reserved.

#include "ATetrisGameMode.h"
#include "ATetrisPlayerController.h"
#include "ATetrisPawn.h"

AATetrisGameMode::AATetrisGameMode()
{
	// no pawn by default
	DefaultPawnClass = AATetrisPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AATetrisPlayerController::StaticClass();
}
