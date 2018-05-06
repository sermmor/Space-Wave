// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerProjectile.h"
#include "AeroHeroGameConstants.h"
#include "PlayerShip.h"
#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"

APlayerProjectile::APlayerProjectile()
{
	TSubclassOf<AActor> MissileClass;
	auto PlayerLoadClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Script/AeroHero.PlayerMissileProjectile"));
	if (PlayerLoadClass.Succeeded())
		MissileClass = PlayerLoadClass.Class;

	if (!this->IsA(MissileClass))
	{
		// Static reference to the mesh to use for the projectile
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/AeroHero/Meshes/PlayerProjectile.PlayerProjectile"));

		// Create mesh component for the projectile sphere
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
		ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
		ProjectileMesh->SetupAttachment(RootComponent);
		ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
		ProjectileMesh->OnComponentHit.AddDynamic(this, &AAeroHeroProjectile::OnHit);		// set up a notification for when this component hits something
		RootComponent = ProjectileMesh;

		// Use a ProjectileMovementComponent to govern this projectile's movement
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
		ProjectileMovement->UpdatedComponent = ProjectileMesh;
		ProjectileMovement->InitialSpeed = 3000.f;
		ProjectileMovement->MaxSpeed = 3000.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = false;
		ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity
	}

	// Die after 0.4 seconds by default
	InitialLifeSpan = ShootLifePlayer;
	// Events.
	OnDestroyed.AddDynamic(this, &APlayerProjectile::OnDestroyGameController);
}

void APlayerProjectile::SetPlayerShip(TWeakObjectPtr<APlayerShip> PlayerShip)
{
	this->PlayerShip = PlayerShip;
}

TWeakObjectPtr<APlayerShip> APlayerProjectile::GetPlayerShip()
{
	return PlayerShip;
}

void APlayerProjectile::OnDestroyGameController(AActor* SelfActor)
{
}
