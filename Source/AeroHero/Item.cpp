// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "PlayerShip.h"
#include "EngineMinimal.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Engine/CollisionProfile.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto PlayerLoadClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Script/AeroHero.PlayerShip"));
	if (PlayerLoadClass.Succeeded())
		PlayerClass = PlayerLoadClass.Class;

	ScorePointsToAdd = 5;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AItem::OnHit);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::PlayerHasGottenItem(TWeakObjectPtr<APlayerShip> OtherActor)
{
	
}

void AItem::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor)
	{
		if (OtherActor->IsA(PlayerClass))
		{
			// Notificate score to player, do something with player and destroy item.
			TWeakObjectPtr<APlayerShip> playerCollide = (APlayerShip *)OtherActor;
			playerCollide->UpdateScore(ScorePointsToAdd);
			PlayerHasGottenItem(playerCollide);
			Destroy();
		}

	}
}
