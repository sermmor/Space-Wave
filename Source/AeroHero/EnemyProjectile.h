// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AeroHeroProjectile.h"
#include "EnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class AEROHERO_API AEnemyProjectile : public AAeroHeroProjectile
{
	GENERATED_BODY()

public:
	AEnemyProjectile();

	uint32 Damage;

protected:

	TSubclassOf<AActor> PlayerClass;
	virtual void PostHit(AActor* OtherActor) override;
	
};
