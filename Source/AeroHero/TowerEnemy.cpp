// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerEnemy.h"
#include "TowerEnemyProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Engine/StaticMesh.h"

// Sets default values for this component's properties
UTowerEnemy::UTowerEnemy()
{
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;
}


FVector UTowerEnemy::PrepareShootDirection()
{
	FVector FireDirection = FVector(ShootDirection.X, ShootDirection.Y, ShootDirection.Z);
	return FireDirection;
}

void UTowerEnemy::SpawnProjectile(UWorld * World, FVector SpawnLocation, FRotator FireRotation)
{
	World->SpawnActor<ATowerEnemyProjectile>(SpawnLocation, FireRotation);
}


