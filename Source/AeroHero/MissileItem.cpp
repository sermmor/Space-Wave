// Fill out your copyright notice in the Description page of Project Settings.

#include "MissileItem.h"
#include "PlayerShip.h"
#include "EngineMinimal.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

AMissileItem::AMissileItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMesh(TEXT("/Game/AeroHero/Meshes/MissileItem.MissileItem"));
	// Create the mesh component
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMeshComponent;
	ItemMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ItemMeshComponent->SetStaticMesh(ItemMesh.Object);

}

void AMissileItem::PlayerHasGottenItem(TWeakObjectPtr<APlayerShip> OtherActor)
{
	OtherActor->SetMissileShootMode();
}


