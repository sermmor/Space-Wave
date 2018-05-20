// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class APlayerShip;

UCLASS()
class AEROHERO_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	int ScorePointsToAdd;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSubclassOf<AActor> PlayerClass;

	virtual void PlayerHasGottenItem(TWeakObjectPtr<APlayerShip> OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit);
	
};
