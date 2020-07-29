// Copyright Epic Games, Inc. All Rights Reserved.

#include "ATetrisPlayerController.h"

AATetrisPlayerController::AATetrisPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
