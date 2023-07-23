// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SnakePlayerPawn.generated.h"

class ASnakeBase;

UENUM()
enum class EMovementDirection
{
	Up,
	Down,
	Left,
	Right
};

UCLASS()
class TASK205_API ASnakePlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnakePlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<ASnakeBase> SnakeActor;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HandlePlayerVerticalInput(float Value);

	UFUNCTION()
	void HandlePlayerHorizontalInput(float Value);
};
