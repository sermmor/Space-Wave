// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AEROHERO_API UEnemy : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	/** Put a 1 or -1 in the shoot direction. */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Game Params")
	//FVector ShootDirection;

	/* Fire a shot in the specified direction */
	virtual void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Choose if is a moment to shoot or not. */
	virtual bool CanShoot();

	/* Get vector with direction to shoot. */
	virtual FVector PrepareShootDirection();

	/* Create a Projectile. */
	virtual void SpawnProjectile(UWorld* World, FVector SpawnLocation, FRotator FireRotation);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;
	
};
