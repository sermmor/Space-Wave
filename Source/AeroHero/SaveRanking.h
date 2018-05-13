// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveRanking.generated.h"

/**
 * 
 */

const FString DefaultSaveSlotName = TEXT("SAeroHeroSaveSlot");
const uint32 DefaultUserIndex = 0;

UCLASS()
class AEROHERO_API USaveRanking : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<FString> NamePlayer;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<int> ScorePlayer;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	USaveRanking();
	
};
