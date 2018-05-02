// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalEnemy.h"
#include "NormalEnemyProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Engine/StaticMesh.h"

UNormalEnemy::UNormalEnemy()
{
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// TODO: Random to shoot init: AÑADIR TIEMPO INICIAL ALEATORIO PARA DISPARAR, PARA QUE ASÍ TODA NAVE ENEMIGA NO COMIENCE DISPARANDO A LA VEZ.

	TimeToShootOportunity = 2.5f; // 2.5 seconds.
	IsOportunityTimerLauched = false;
	IsInAShootOportunity = false; // It begins not shooting.
}

FVector UNormalEnemy::PrepareShootDirection()
{
	return Super::PrepareShootDirection();
}

void UNormalEnemy::SpawnProjectile(UWorld * World, FVector SpawnLocation, FRotator FireRotation)
{
	World->SpawnActor<ANormalEnemyProjectile>(SpawnLocation, FireRotation);
}

void UNormalEnemy::DoAfterInvertShoot()
{
}

void UNormalEnemy::InvertShoot()
{
	IsInAShootOportunity = !IsInAShootOportunity;
	IsOportunityTimerLauched = false;
	DoAfterInvertShoot();
}

bool UNormalEnemy::CanShoot()
{
	if (!IsOportunityTimerLauched)
	{
		AActor * parent = Super::GetOwner();
		UWorld* world = parent->GetWorld();

		// Launch timer and mark timer like launcher.
		world->GetTimerManager().SetTimer(TimerHandle_ShotOportunityExpired, this, &UNormalEnemy::InvertShoot, TimeToShootOportunity);
		IsOportunityTimerLauched = true;

		world = NULL;
		parent = NULL;
	}
	return IsInAShootOportunity;
}

// Called every frame
void UNormalEnemy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// After shoot and a lot of issues.
	DoAMove();
}

void UNormalEnemy::DoAMove()
{
	// TODO: Move ship enemy to camera.
}
