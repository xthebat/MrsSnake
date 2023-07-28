// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"

#include "DrawDebugHelpers.h"
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
	SetActorTickEnabled(Enabled); // Disable ticks if snake is invalid

	// Just in case set current direction if player press something before Tick
	// ForwardVector = PendingDirection;
}

UChildActorComponent* ASnakeBase::AddElement(UStaticMesh* Mesh, TOptional<FVector> Location)
{
	const auto Component = NewObject<UChildActorComponent>(this);
	Component->RegisterComponent();

	Component->SetChildActorClass(ASnakeBaseElement::StaticClass());
	Component->CreateChildActor();

	const auto SnakeElement = Cast<ASnakeBaseElement>(Component->GetChildActor());

	SnakeElement->SetStaticMesh(Mesh);

	Component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Location %s"),
		*Location->ToString())

	if (Location.IsSet())
		Component->SetRelativeLocation(*Location);

	return Component;
}

void ASnakeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (HeadStaticMesh == nullptr || BodyStaticMesh == nullptr)
		return;

	SnakeElementSize = {};
	SnakeComponents.SetNum(SnakeInitialSize);

	const auto Direction = FVector{-1.0f, 0.0f, 0.0f};

	for (int i = 0; i < SnakeInitialSize; i++)
	{
		const auto Mesh = i == 0 ? HeadStaticMesh : BodyStaticMesh;
		const auto Box = Mesh->GetBoundingBox();

		SnakeElementSize = (Box.Max - Box.Min).X;
		const auto FloatOffset = i * (SnakeElementSize + SnakeElementSpace);
		const auto VectorOffset = Direction * FloatOffset;

		const auto Component = AddElement(Mesh, VectorOffset);

		Component->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		SnakeComponents[i] = Component;
	}
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick disabled in begin play if no components so snake is valid and have at least one component

	FRotator Rotator = {0.0f, 0.0f, 0.0f};

	switch (PendingDirection)
	{
	case EMovementDirection::Up:
		Rotator.Pitch += 90.0f;
		break;
	case EMovementDirection::Down:
		Rotator.Pitch -= 90.0f;
		break;
	case EMovementDirection::Left:
		Rotator.Yaw += 90.0f;
		break;
	case EMovementDirection::Right:
		Rotator.Yaw -= 90.0f;
		break;
	case EMovementDirection::Forward:
		break;
	}

	if (IsGrow)
	{
		// IsGrow = false;
		const auto LastComponent = SnakeComponents.Last();
		const auto Transform = LastComponent->GetRelativeTransform();
		const auto GrownComponent = AddElement(BodyStaticMesh, Transform.GetLocation());
		SnakeComponents.Add(GrownComponent);
	}

	for (int i = SnakeComponents.Num() - 1; i > 0; i--)
	{
		const auto PreviousTransform = SnakeComponents[i - 1]->GetRelativeTransform();
		SnakeComponents[i]->SetRelativeTransform(PreviousTransform);
	}

	SnakeComponents[0]->AddLocalRotation(Rotator);

	const auto ForwardVector = SnakeComponents[0]->GetForwardVector();

	DrawDebugLine(GetWorld(),
	              SnakeComponents[0]->GetComponentLocation(),
	              SnakeComponents[0]->GetComponentLocation() + ForwardVector * 1000,
	              FColor::Emerald, false, SnakeTickTime, 0, 10);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Rotator: %s ForwardVector %s"),
		*Rotator.ToString(),
		*ForwardVector.ToString());

	const auto Scale = GetActorScale3D();
	const auto Spacing = SnakeElementSize + SnakeElementSpace;
	const auto Offset = Scale * Spacing * ForwardVector;

	SnakeComponents[0]->AddWorldOffset(Offset);

	PendingDirection = EMovementDirection::Forward;
}

void ASnakeBase::SetDirection(EMovementDirection Direction) { PendingDirection = Direction; }
