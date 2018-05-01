// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerEnemy.h"
#include "TowerEnemyProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values for this component's properties
UTowerEnemy::UTowerEnemy()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Weapon
	GunOffset = FVector(200.f, 0.f, 0.f); // TODO: CHANGE DIRECTION VARIABLE (+ shoot right, - shoot left).
	FireRate = 0.1f;
	bCanFire = true;
}


// Called when the game starts
void UTowerEnemy::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTowerEnemy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// Try and fire a shot
	const FVector FireDirection = FVector(ShootDirection.X, ShootDirection.Y, ShootDirection.Z);//FVector(0.f, 1.f, 0.f);
	FireShot(FireDirection);
}


void UTowerEnemy::FireShot(FVector FireDirection)
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
				//UE_LOG(LogTemp, Warning, TEXT("-----> CREATED AT (%f, %f, %f) WITH ROTATION (%f, %f, %f)"),
				//	SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z, FireDirection.X, FireDirection.Y, FireDirection.Z);
				World->SpawnActor<ATowerEnemyProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &UTowerEnemy::ShotTimerExpired, FireRate);

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

void UTowerEnemy::ShotTimerExpired()
{
	bCanFire = true;
}


