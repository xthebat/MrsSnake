// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreComponent.h"

#include "MrsSnake/Character/MrsSnakeBase.h"

void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();
	MaxScoreAmount = FMath::Max(1, MaxScoreAmount);
	ActualScoreAmount = IsRandomizeScore ? FMath::RandRange(1, MaxScoreAmount) : MaxScoreAmount;
}

void UScoreComponent::Affect(AMrsSnakeBase* Snake) const
{
	Snake->IncreaseScore(ActualScoreAmount);
}
