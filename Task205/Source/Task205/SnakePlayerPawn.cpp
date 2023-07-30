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

	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &ASnakePlayerPawn::HandlePlayerVerticalUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &ASnakePlayerPawn::HandlePlayerVerticalDown);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &ASnakePlayerPawn::HandlePlayerVerticalLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &ASnakePlayerPawn::HandlePlayerVerticalRight);
}

void ASnakePlayerPawn::HandlePlayerVerticalUp()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Up);
}

void ASnakePlayerPawn::HandlePlayerVerticalDown()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Down);
}

void ASnakePlayerPawn::HandlePlayerVerticalLeft()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Left);
}

void ASnakePlayerPawn::HandlePlayerVerticalRight()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Right);
}