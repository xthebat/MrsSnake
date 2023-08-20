// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MrsSnakeInfo.generated.h"

/**
 *
 */
UCLASS()
class MRSSNAKE_API UMrsSnakeInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int Score = {};

	UPROPERTY(BlueprintReadOnly)
	float Health = {};

	UPROPERTY(BlueprintReadOnly)
	FString Message = {};
};
