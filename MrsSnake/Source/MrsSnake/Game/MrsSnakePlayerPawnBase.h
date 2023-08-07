// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MrsSnakePlayerPawnBase.generated.h"

class UCameraComponent;
class AMrsSnakeBase;

UENUM()
enum class EMovementDirection
{
	Up,
	Down,

	Left,
	Right,

	Forward
};

UCLASS()
class MRSSNAKE_API AMrsSnakePlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMrsSnakePlayerPawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<AMrsSnakeBase> SnakeActor;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HandlePlayerVerticalUp();

	UFUNCTION()
	void HandlePlayerVerticalDown();

	UFUNCTION()
	void HandlePlayerVerticalLeft();

	UFUNCTION()
	void HandlePlayerVerticalRight();
};
