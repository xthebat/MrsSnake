// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourComponent.h"
#include "ScoreComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API UScoreComponent : public UBehaviourComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	int MaxScoreAmount = 1;

	UPROPERTY(EditAnywhere)
	bool IsRandomizeScore = false;

	int ActualScoreAmount = {};
public:
	virtual void BeginPlay() override;

	virtual void Affect(AMrsSnakeBase* Snake) const override;

	UFUNCTION(BlueprintCallable)
	int GetActualScoreAmount() const { return ActualScoreAmount; }

	UFUNCTION(BlueprintCallable)
	int GetMaxScoreAmount() const { return MaxScoreAmount; }
};
