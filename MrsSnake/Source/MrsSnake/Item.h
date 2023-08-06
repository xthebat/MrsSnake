﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UWidgetComponent;

UCLASS()
class MRSSNAKE_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditDefaultsOnly)
	float SelfDestructionTime = 30.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle SelfDestructionHandle;

	float SelfDestructionRemain = {};

	float SelfDestructionTick = {};

	float GetSelfDestructionTick() const;

	UFUNCTION()
	void OnSelfDestructionTick();
};