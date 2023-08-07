// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfo.generated.h"

/**
 *
 */
UCLASS()
class MRSSNAKE_API UItemInfo : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	float Health = {};

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth = {};
};
