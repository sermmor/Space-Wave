// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/ActorComponent.h"
#include "NormalEnemy.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AEROHERO_API UNormalEnemy : public UEnemy
{
	GENERATED_BODY()
	
public:
	UNormalEnemy();
	

protected:
	/* Time until decide whether shoot or not. */
	float TimeToShootOportunity;

	virtual FVector PrepareShootDirection() override;

	virtual void SpawnProjectile(UWorld* World, FVector SpawnLocation, FRotator FireRotation) override;

	virtual bool CanShoot() override;

	// Do something after choose whether shoot or not.
	virtual void DoAfterInvertShoot();

	// Do a movement.
	virtual void DoAMove();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotOportunityExpired;

	// If the oportunity timer was launched.
	bool IsOportunityTimerLauched;
	
	// If now decide to shoot.
	bool IsInAShootOportunity;

	// Invert shoot oportunity (shoot or not).
	void InvertShoot();
};
