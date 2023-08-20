// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawner.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class USpawner : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MRSSNAKE_API ISpawner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FVector> GetLocations(
		const TMap<FString, FString>& Parameters,
		FVector BottomLeft,
		FVector TopRight,
		float Grid);
};
