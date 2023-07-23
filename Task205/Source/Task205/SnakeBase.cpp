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
}

void ASnakeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SnakeBaseElementClass == nullptr)
		return;

	const auto Rotation = Transform.GetRotation();

	auto Origin = FVector{};
	auto Extents = FVector{};

	for (int i = 0; i < SnakeInitialSize; i++)
	{
		const auto Component = NewObject<UChildActorComponent>(this);
		Component->RegisterComponent();

		Component->SetChildActorClass(SnakeBaseElementClass);
		Component->CreateChildActor();

		const auto SnakeElement = Component->GetChildActor();
		SnakeElement->GetActorBounds(false, Origin, Extents, true);

		const auto SnakeElementSize = Extents.X * 2.0f;
		const auto SnakeElementOffset = i * (SnakeElementSize + SnakeElementSpace);
		const auto SnakeElementVector = FVector{SnakeElementOffset, 0, 0};
		const auto Rotated = Rotation.RotateVector(SnakeElementVector);

		Component->SetWorldTransform(Transform);
		Component->AddRelativeLocation(Rotated);

		Component->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	}
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
