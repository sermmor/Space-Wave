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

	// Random to first shoot, so all NormalEnemy don't shoot in the same time.
	// 0 <= TimeToFirstShoot <= 2.4
	TimeToFirstShoot = (((float) FMath::RandRange(0, 8)) / 10.f) * 3.f;

	// Another shoot vars
	TimeToShootOportunity = 2.5f; // 2.5 seconds.
	IsOportunityTimerLauched = false;
	IsInAShootOportunity = false; // It begins not shooting.
	IsFirstShoot = true;

	// Movement
	MoveSpeed = 500.0f;
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

		if (IsFirstShoot)
		{
			world->GetTimerManager().SetTimer(TimerHandle_ShotOportunityExpired, this, &UNormalEnemy::InvertShoot, TimeToFirstShoot);
			IsFirstShoot = false;
		}
		else
		{
			world->GetTimerManager().SetTimer(TimerHandle_ShotOportunityExpired, this, &UNormalEnemy::InvertShoot, TimeToShootOportunity);
		}
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
	if (CanMove())
		DoAMove(DeltaTime);
}

FVector UNormalEnemy::GetMoveDirection()
{
	FVector MoveDirection = FVector(-1, 0.f, 0.f).GetClampedToMaxSize(1.0f);
	return MoveDirection;
}

bool UNormalEnemy::CanMove()
{
	return IsEnemyEnabled();
}

void UNormalEnemy::DoAMove(float DeltaTime)
{
	FVector MoveDirection = GetMoveDirection();

	FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

	AActor * parent = Super::GetOwner();
	FVector currentLocation = parent->GetActorLocation();
	currentLocation.X += Movement.X;
	currentLocation.Y += Movement.Y;
	currentLocation.Z += Movement.Z;
	parent->SetActorLocation(currentLocation);

}

