// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"

#include "SnakeBaseElement.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(SnakeTickTime);
	const auto Enabled = SnakeElementSize > 0.0f && SnakeComponents.Num() > 0;
	SetActorTickEnabled(Enabled);
}

void ASnakeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SnakeBaseElementClass == nullptr)
		return;

	const auto Scale = Transform.GetScale3D();
	const auto Rotation = Transform.GetRotation();

	auto Origin = FVector{};
	auto Extents = FVector{};

	SnakeElementSize = {};
	SnakeComponents.SetNum(SnakeInitialSize);
	DirectionVector = {1.0f, 0.0f, 0.0f};

	for (int i = 0; i < SnakeInitialSize; i++)
	{
		const auto Component = NewObject<UChildActorComponent>(this);
		Component->RegisterComponent();

		Component->SetChildActorClass(SnakeBaseElementClass);
		Component->CreateChildActor();

		const auto SnakeElement = Component->GetChildActor();
		SnakeElement->GetActorBounds(false, Origin, Extents, true);

		SnakeElementSize = Extents.X * 2.0f;
		const auto SnakeElementOffset = i * (SnakeElementSize + SnakeElementSpace);
		// TODO: Add function to calc SnakeElementVector
		const auto SnakeElementVector = Scale * DirectionVector * SnakeElementOffset;

		Component->SetWorldTransform(Transform);
		Component->AddRelativeLocation(Rotation.RotateVector(SnakeElementVector));

		Component->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		SnakeComponents[i] = Component;
	}

	DirectionVector = -DirectionVector;
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick disabled in begin play if no components

	const auto Transform = SnakeComponents[0]->GetComponentTransform();
	const auto SnakeElementOffset = SnakeElementSize + SnakeElementSpace;

	const auto Direction = Transform.GetScale3D() * SnakeElementOffset * DirectionVector;

	const auto Offset = Transform.Rotator().RotateVector(Direction);

	for (int i = SnakeComponents.Num() - 1; i > 0; i--)
	{
		const auto Location = SnakeComponents[i - 1]->GetComponentLocation();
		SnakeComponents[i]->SetWorldLocation(Location);
	}

	SnakeComponents[0]->AddRelativeLocation(Offset);
}

void ASnakeBase::SetDirection(EMovementDirection Direction)
{
	FVector NewDirectionVector = {0.0f, 0.0f, 0.0f};

	switch (Direction)
	{
	case EMovementDirection::Up:
		NewDirectionVector.Y = 1.0f;
		break;
	case EMovementDirection::Down:
		NewDirectionVector.Y = -1.0f;
		break;
	case EMovementDirection::Left:
		NewDirectionVector.X = 1.0f;
		break;
	case EMovementDirection::Right:
		NewDirectionVector.X = -1.0f;
		break;
	}

	if (DirectionVector != -NewDirectionVector)
		DirectionVector = NewDirectionVector;
}
