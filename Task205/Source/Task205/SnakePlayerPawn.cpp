// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerPawn.h"

#include "SnakeBase.h"
#include "Engine/Classes/Camera/CameraComponent.h"

UENUM()
enum class EMovementDirection
{
	Up,
	Down,
	Left,
	Right
};

// Sets default values
ASnakePlayerPawn::ASnakePlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCameraRoot"));
}

// Called when the game starts or when spawned
void ASnakePlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation({-90.0f, 0, 0});
	// CreateSnakeActor();
}

void ASnakePlayerPawn::CreateSnakeActor()
{
	const auto Transform = FTransform{};
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, Transform);
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
}
