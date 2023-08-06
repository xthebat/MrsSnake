// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Apple.generated.h"

UCLASS()
class MRSSNAKE_API AApple : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AApple();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
