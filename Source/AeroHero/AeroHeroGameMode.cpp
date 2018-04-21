// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AeroHeroGameMode.h"
#include "AeroHeroPawn.h"

AAeroHeroGameMode::AAeroHeroGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AAeroHeroPawn::StaticClass();
}

