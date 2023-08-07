// Fill out your copyright notice in the Description page of Project Settings.


#include "MrsSnakePlayerPawnBase.h"
#include "MrsSnake/Character/MrsSnakeBase.h"


// Sets default values
AMrsSnakePlayerPawnBase::AMrsSnakePlayerPawnBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
}

// Called when the game starts or when spawned
void AMrsSnakePlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMrsSnakePlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMrsSnakePlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AMrsSnakePlayerPawnBase::HandlePlayerVerticalUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AMrsSnakePlayerPawnBase::HandlePlayerVerticalDown);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AMrsSnakePlayerPawnBase::HandlePlayerVerticalLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AMrsSnakePlayerPawnBase::HandlePlayerVerticalRight);
}

void AMrsSnakePlayerPawnBase::HandlePlayerVerticalUp()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Up);
}

void AMrsSnakePlayerPawnBase::HandlePlayerVerticalDown()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Down);
}

void AMrsSnakePlayerPawnBase::HandlePlayerVerticalLeft()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Left);
}

void AMrsSnakePlayerPawnBase::HandlePlayerVerticalRight()
{
	if (SnakeActor.IsValid())
		SnakeActor->SetDirection(EMovementDirection::Right);
}