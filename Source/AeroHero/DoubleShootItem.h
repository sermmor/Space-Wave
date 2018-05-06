// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DoubleShootItem.generated.h"

/**
 * 
 */
UCLASS()
class AEROHERO_API ADoubleShootItem : public AItem
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMeshComponent;
	
public:
	ADoubleShootItem();

protected:
	virtual void PlayerHasGottenItem(TWeakObjectPtr<APlayerShip> OtherActor) override;
	
};
