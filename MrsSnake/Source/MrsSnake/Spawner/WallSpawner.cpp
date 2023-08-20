// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpawner.h"

static int GetIntParameter(const TMap<FString, FString>& Parameters, const char* Name, int Default = 0)
{
	const auto Entry = Parameters.Find(Name);
	if (Entry == nullptr)
		return Default;

	const auto Value = *Entry;

	if (Value.Len() == 0)
		return Default;

	// no embedded conversion to int ... no santization thou ...
	const auto Result = FCString::Atoi(*Value);

	if (Result < 0)
		return 0;

	return Result;
}

TArray<FVector> UWallSpawner::GetLocations_Implementation(
	const TMap<FString, FString>& Parameters,
	FVector BottomLeft,
	FVector TopRight,
	float Grid)
{
	const auto Base = FVector{
		FMath::FRandRange(BottomLeft.X, TopRight.X),
		FMath::FRandRange(BottomLeft.Y, TopRight.Y),
		0.0f,
	}.GridSnap(Grid);

	const auto MinWallSize = GetIntParameter(Parameters, "MinWallSize", 2);
	const auto MaxWallSize = GetIntParameter(Parameters, "MaxWallSize", 5);

	const auto Count = FMath::RandRange(MinWallSize, MaxWallSize);
	const auto Angle = FMath::GridSnap(FMath::FRandRange(0.0f, 360.0f), 90.0f);

	const auto Direction = Grid * FRotator(0.0f, Angle, 0.0f).RotateVector(FVector::XAxisVector);

	TArray<FVector> Result = {Base};

	Result.Reserve(Count);

	for (int Index = 1; Index < Count; Index++)
		Result.Add(Base + Direction * Index);

	return Result;
}
