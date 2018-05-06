// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AeroHeroProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config=Game)
class AAeroHeroProjectile : public AActor
{
	GENERATED_BODY()

public:
	int Damage;

	AAeroHeroProjectile();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	int GetDamage();
protected:
	virtual void PostHit(AActor* OtherActor);
};

