// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyProjectile.h"
#include "AeroHeroGameConstants.h"
#include "EngineMinimal.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AEnemyProjectile::AEnemyProjectile()
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/SpaceWave/Meshes/EnemyProjectile.EnemyProjectile"));

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

	// References for collisions.
	auto PlayerLoadClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Script/AeroHero.PlayerShip"));
	if (PlayerLoadClass.Succeeded())
		PlayerClass = PlayerLoadClass.Class;

	Damage = DefaultDamageEnemyProjectile;
}

void AEnemyProjectile::PostHit(AActor * OtherActor)
{
	Super::PostHit(OtherActor);

	if (OtherActor)
	{
		if (OtherActor->IsA(PlayerClass))
		{
			// Notificate damage to player and destroy enemy.
			UGameplayStatics::ApplyDamage(OtherActor, Damage, NULL, Super::GetOwner(), UDamageType::StaticClass());
		}

	}
}
