// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyProjectile.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UEnemy::UEnemy()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Weapon
	GunOffset = FVector(200.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}




// Called when the game starts
void UEnemy::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEnemy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanShoot())
	{
		// Try and fire a shot
		const FVector FireDirection = PrepareShootDirection();
		FireShot(FireDirection);
	}
}

bool UEnemy::CanShoot()
{
	return true;
}

FVector UEnemy::PrepareShootDirection()
{
	// Default mode: shoot to player camera.
	FVector FireDirection = FVector(-1.f, 0.f, 0.f);
	return FireDirection;
}

void UEnemy::SpawnProjectile(UWorld* World, FVector SpawnLocation, FRotator FireRotation)
{
	World->SpawnActor<AEnemyProjectile>(SpawnLocation, FireRotation);
}

void UEnemy::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			AActor * parent = Super::GetOwner();
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = parent->GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = parent->GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				SpawnProjectile(World, SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &UEnemy::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, parent->GetActorLocation());
			}

			bCanFire = false;
			parent = NULL;
		}
	}
}

void UEnemy::ShotTimerExpired()
{
	bCanFire = true;
}
