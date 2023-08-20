// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomSpawner.h"


TArray<FVector> URandomSpawner::GetLocations_Implementation(
	const TMap<FString, FString>& Parameters,
	FVector BottomLeft,
	FVector TopRight,
	float Grid)
{
	return {
		FVector{
			FMath::FRandRange(BottomLeft.X, TopRight.X),
			// 0.0f,
			FMath::FRandRange(BottomLeft.Y, TopRight.Y),
			0.0f,
		}.GridSnap(Grid)
	};
}
