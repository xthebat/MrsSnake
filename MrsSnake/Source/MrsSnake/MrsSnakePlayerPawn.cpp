// Fill out your copyright notice in the Description page of Project Settings.


#include "MrsSnakePlayerPawn.h"

#include "MrsSnakeBase.h"

// Sets default values
AMrsSnakePlayerPawn::AMrsSnakePlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
}

// Called when the game starts or when spawned
void AMrsSnakePlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMrsSnakePlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMrsSnakePlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AMrsSnakePlayerPawn::HandlePlayerVerticalUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AMrsSnakePlayerPawn::HandlePlayerVerticalDown);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AMrsSnakePlayerPawn::HandlePlayerVerticalLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AMrsSnakePlayerPawn::HandlePlayerVerticalRight);
}

void AMrsSnakePlayerPawn::HandlePlayerVerticalUp()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Up);
}

void AMrsSnakePlayerPawn::HandlePlayerVerticalDown()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Down);
}

void AMrsSnakePlayerPawn::HandlePlayerVerticalLeft()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Left);
}

void AMrsSnakePlayerPawn::HandlePlayerVerticalRight()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Right);
}