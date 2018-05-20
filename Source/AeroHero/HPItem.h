// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HPItem.generated.h"

/**
 * 
 */
UCLASS()
class AEROHERO_API AHPItem : public AItem
{
	GENERATED_BODY()
	
	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMeshComponent;
	
public:
	AHPItem();

protected:
	int lifeToRecover;

	virtual void PlayerHasGottenItem(TWeakObjectPtr<APlayerShip> OtherActor) override;

};
