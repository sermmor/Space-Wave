// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerProjectile.h"
#include "PlayerMissileProjectile.generated.h"

/**
 * 
 */
UCLASS()
class AEROHERO_API APlayerMissileProjectile : public APlayerProjectile
{
	GENERATED_BODY()
	
	/*
	/** Sphere collision component * /
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MissileMesh;

	/** Projectile movement component * /
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* MissileMovement;
	*/

	
public:
	APlayerMissileProjectile();
	
};
