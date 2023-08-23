// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MrsSnakeInfoBase.generated.h"

/**
 *
 */
UCLASS()
class MRSSNAKE_API UMrsSnakeInfoBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int Score = 0;

	UPROPERTY(BlueprintReadOnly)
	int TotalLifeTime = 0;

	UPROPERTY(BlueprintReadOnly)
	float Health = 1.0;

	UPROPERTY(BlueprintReadOnly)
	FString Message = {};
};
