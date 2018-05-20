// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RotateTowerEnemy.h"
#include "DoubleRotateTowerEnemy.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AEROHERO_API UDoubleRotateTowerEnemy : public URotateTowerEnemy
{
	GENERATED_BODY()
	

public:
	UDoubleRotateTowerEnemy();

	void Shot2TimerExpired();

protected:
	// Shoot to the opposite direction.
	virtual FVector InvertDirection(FVector directionToInvert);
	virtual void FireShot2(FVector FireDirection);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/* Flag to control firing  */
	uint32 bCanFire2 : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_Shot2TimerExpired;
	
	
};
