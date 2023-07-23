// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakePlayerPawn.h"
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

	UPROPERTY(EditAnywhere)
	int SnakeInitialSize = 5;

	UPROPERTY(EditAnywhere)
	float SnakeElementSpace = 10.0;

	UPROPERTY(EditAnywhere)
	float SnakeTickTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeBaseElement> SnakeBaseElementClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	TArray<UChildActorComponent*> SnakeComponents = {};

	UPROPERTY()
	float SnakeElementSize = {};

	UPROPERTY()
	FVector DirectionVector = {};
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDirection(EMovementDirection Direction);
};
