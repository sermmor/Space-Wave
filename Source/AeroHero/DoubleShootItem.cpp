// Fill out your copyright notice in the Description page of Project Settings.

#include "DoubleShootItem.h"
#include "PlayerShip.h"
#include "EngineMinimal.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

ADoubleShootItem::ADoubleShootItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMesh(TEXT("/Game/Geometry/Meshes/DoubleShootItem.DoubleShootItem"));
	// Create the mesh component
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMeshComponent;
	ItemMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ItemMeshComponent->SetStaticMesh(ItemMesh.Object);

}

void ADoubleShootItem::PlayerHasGottenItem(TWeakObjectPtr<APlayerShip> OtherActor)
{
	OtherActor->SetDoubleShootMode();
}
