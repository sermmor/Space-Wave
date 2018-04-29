// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AeroHeroGameMode.h"
#include "AeroHeroPawn.h"
#include "PlayerShip.h"
#include "PlayerControllerAeroHero.h"

AAeroHeroGameMode::AAeroHeroGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = APlayerShip::StaticClass();//AAeroHeroPawn::StaticClass();
	//PlayerControllerClass = APlayerControllerAeroHero::StaticClass();
}

