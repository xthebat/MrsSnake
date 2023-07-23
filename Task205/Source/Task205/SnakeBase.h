// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeBase.generated.h"

class ASnakeBaseElement;

UCLASS()
class TASK205_API ASnakeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(BlueprintReadWrite)
	int SnakeInitialSize = 1;

	UPROPERTY(BlueprintReadWrite)
	float SnakeElementSpace = {};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeBaseElement> SnakeBaseElementClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	TArray<ASnakeBaseElement*> SnakeElements = {};
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
