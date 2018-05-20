// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerEnemyProjectile.h"
#include "AeroHeroGameConstants.h"

ATowerEnemyProjectile::ATowerEnemyProjectile()
{
	// Die after 0.2 seconds by default
	InitialLifeSpan = ShootLifeTowerEnemy;
}

