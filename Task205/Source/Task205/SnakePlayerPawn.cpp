// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerPawn.h"

#include "SnakeBase.h"
#include "Engine/Classes/Camera/CameraComponent.h"

// Sets default values
ASnakePlayerPawn::ASnakePlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
}

// Called when the game starts or when spawned
void ASnakePlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakePlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASnakePlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Vertical", this, &ASnakePlayerPawn::HandlePlayerVerticalInput);
	PlayerInputComponent->BindAxis("Horizontal", this, &ASnakePlayerPawn::HandlePlayerHorizontalInput);
}

void ASnakePlayerPawn::HandlePlayerVerticalInput(float Value)
{
	if (SnakeActor.IsValid() && Value != 0.0f)
	{
		const auto Direction = Value > 0.0f ? EMovementDirection::Up : EMovementDirection::Down;
		SnakeActor->SetDirection(Direction);
	}
}

void ASnakePlayerPawn::HandlePlayerHorizontalInput(float Value)
{
	if (SnakeActor.IsValid() && Value != 0.0f)
	{
		const auto Direction = Value > 0.0f ? EMovementDirection::Left : EMovementDirection::Right;
		SnakeActor->SetDirection(Direction);
	}
}