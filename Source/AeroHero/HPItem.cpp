// Fill out your copyright notice in the Description page of Project Settings.

#include "HPItem.h"
#include "PlayerShip.h"
#include "EngineMinimal.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

AHPItem::AHPItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMesh(TEXT("/Game/Geometry/Meshes/HPItem.HPItem"));
	// Create the mesh component
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMeshComponent;
	ItemMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ItemMeshComponent->SetStaticMesh(ItemMesh.Object);
	
	lifeToRecover = 20;
}

void AHPItem::PlayerHasGottenItem(TWeakObjectPtr<APlayerShip> OtherActor)
{
	OtherActor->GetHPItem(lifeToRecover);
}
