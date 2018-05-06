// Copyright 1998-2018 Epic Games, Inc. All Rights Reserve

#include "AeroHeroProjectile.h"
#include "AeroHeroGameConstants.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"

AAeroHeroProjectile::AAeroHeroProjectile() 
{
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	
	// Damage.
	Damage = DefaultDamageProjectile;
}

void AAeroHeroProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
	}
	PostHit(OtherActor);

	Destroy();
}

void AAeroHeroProjectile::PostHit(AActor * OtherActor)
{

}

int AAeroHeroProjectile::GetDamage()
{
	//UE_LOG(LogTemp, Warning, TEXT("(damage value = %d"), Damage);
	return Damage;
}
