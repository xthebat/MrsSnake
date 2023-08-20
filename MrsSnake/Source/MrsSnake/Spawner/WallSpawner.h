// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "UObject/Object.h"
#include "WallSpawner.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MRSSNAKE_API UWallSpawner : public UObject, public ISpawner
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FVector> GetLocations(
		const TMap<FString, FString>& Parameters,
		FVector BottomLeft,
		FVector TopRight,
		float Grid);

	virtual TArray<FVector> GetLocations_Implementation(
		const TMap<FString, FString>& Parameters,
		FVector BottomLeft,
		FVector TopRight,
		float Grid) override;
};
