// Fill out your copyright notice in the Description page of Project Settings.

#include "DoubleTowerEnemy.h"
#include "EngineMinimal.h"
#include "PhysicsEngine/PhysicsAsset.h"

UDoubleTowerEnemy::UDoubleTowerEnemy()
{
	bCanFire2 = true;
}

FVector UDoubleTowerEnemy::InvertDirection(FVector directionToInvert)
{
	return FVector(-directionToInvert.X, -directionToInvert.Y, -directionToInvert.Z);
}

void UDoubleTowerEnemy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsEnemyEnabled() && CanShoot())
	{
		// Try and fire another shot.
		const FVector FireDirectionInv = InvertDirection(PrepareShootDirection());
		FireShot2(FireDirectionInv);
	}
}

void UDoubleTowerEnemy::FireShot2(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire2 == true)
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

			bCanFire2 = false;
			World->GetTimerManager().SetTimer(TimerHandle_Shot2TimerExpired, this, &UDoubleTowerEnemy::Shot2TimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, parent->GetActorLocation());
			}

			bCanFire2 = false;
			parent = NULL;
		}
	}
}

void UDoubleTowerEnemy::Shot2TimerExpired()
{
	bCanFire2 = true;
}
