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

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
public:
	APlayerProjectile();
	
	void SetPlayerShip(TWeakObjectPtr<APlayerShip> PlayerShip);
	TWeakObjectPtr<APlayerShip> GetPlayerShip();

	// Executed when component destroyed.

	UFUNCTION()
	virtual void OnDestroyGameController(AActor* SelfActor);

	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UPROPERTY()
	TWeakObjectPtr<APlayerShip> PlayerShip;
	
};
