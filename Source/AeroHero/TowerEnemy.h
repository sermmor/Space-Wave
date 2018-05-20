// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/ActorComponent.h"
#include "TowerEnemy.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AEROHERO_API UTowerEnemy : public UEnemy
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTowerEnemy();

	/** Put a 1 or -1 in the shoot direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Game Params")
	FVector ShootDirection;

protected:
	virtual FVector PrepareShootDirection() override;

	virtual void SpawnProjectile(UWorld* World, FVector SpawnLocation, FRotator FireRotation) override;
	
};
