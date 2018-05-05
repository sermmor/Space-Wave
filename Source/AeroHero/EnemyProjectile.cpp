// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyProjectile.h"
#include "EngineMinimal.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsAsset.h"

AEnemyProjectile::AEnemyProjectile()
{
	auto PlayerLoadClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Script/AeroHero.PlayerShip"));
	if (PlayerLoadClass.Succeeded())
		PlayerClass = PlayerLoadClass.Class;

	Damage = 1;
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
