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

	if (Sna)

	for (int i = 0; i < SnakeInitialSize; i++)
	{
		const auto SnakeElement = GetWorld()->SpawnActor<ASnakeBaseElement>(SnakeBaseElementClass, Transform);
		auto Origin = FVector{};
		auto Extents = FVector{};
		SnakeElement->GetActorBounds(false, Origin, Extents, true);
		const auto SnakeElementSize = Extents.X * 2.0f;
		const auto SnakeElementOffset = SnakeElements.Num() * (SnakeElementSize + SnakeElementSpace);
		const auto SnakeElementVector = FVector{SnakeElementOffset, 0.0f, 0.0f};
		const auto SnakeElementTransform = FTransform{SnakeElementVector};
		SnakeElement->SetActorTransform(SnakeElementTransform);
		SnakeElement->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		SnakeElements.Add(SnakeElement);
	}
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
