// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalEnemyProjectile.h"
#include "AeroHeroGameConstants.h"

ANormalEnemyProjectile::ANormalEnemyProjectile()
{
	// Die after 0.4 seconds by default
	InitialLifeSpan = ShootLifeNormalEnemy;

}