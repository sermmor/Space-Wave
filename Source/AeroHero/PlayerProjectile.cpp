// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerProjectile.h"
#include "AeroHeroGameConstants.h"
#include "PlayerShip.h"
#include "Engine.h"

APlayerProjectile::APlayerProjectile()
{
	// Die after 0.4 seconds by default
	InitialLifeSpan = ShootLifePlayer;
	// Events.
	OnDestroyed.AddDynamic(this, &APlayerProjectile::OnDestroyGameController);
}

void APlayerProjectile::SetPlayerShip(TWeakObjectPtr<APlayerShip> PlayerShip)
{
	this->PlayerShip = PlayerShip;
}

TWeakObjectPtr<APlayerShip> APlayerProjectile::GetPlayerShip()
{
	return PlayerShip;
}

void APlayerProjectile::OnDestroyGameController(AActor* SelfActor)
{
	/*if (PlayerShip != NULL)
		PlayerShip = NULL;*/
}