// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AeroHeroProjectile.h"
#include "PlayerProjectile.generated.h"

class APlayerShip;
/**
 * 
 */
UCLASS()
class AEROHERO_API APlayerProjectile : public AAeroHeroProjectile
{
	GENERATED_BODY()
	
public:
	APlayerProjectile();
	
	void SetPlayerShip(TWeakObjectPtr<APlayerShip> PlayerShip);
	TWeakObjectPtr<APlayerShip> GetPlayerShip();

	// Executed when component destroyed.

	UFUNCTION()
	virtual void OnDestroyGameController(AActor* SelfActor);

protected:
	UPROPERTY()
	TWeakObjectPtr<APlayerShip> PlayerShip;
	
};
