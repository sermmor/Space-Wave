// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerProjectile.h"
#include "AeroHeroGameConstants.h"

APlayerProjectile::APlayerProjectile()
{
	// Die after 0.4 seconds by default
	InitialLifeSpan = ShootLifePlayer;
}

