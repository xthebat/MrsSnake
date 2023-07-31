// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"

#include "DrawDebugHelpers.h"
#include "SnakeBaseElement.h"
#include "SnakeGameModeBase.h"
#include "Apple.h"
#include "Kismet/GameplayStatics.h"

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
	SetActorTickEnabled(SnakeComponents.Num() > 1); // Disable ticks if snake is invalid
}

FRotator ASnakeBase::Direction2Rotator(EMovementDirection Direction)
{
	FRotator Rotator = {0.0f, 0.0f, 0.0f};

	switch (Direction)
	{
	case EMovementDirection::Up:
		Rotator.Pitch += 90.0f;
		break;
	case EMovementDirection::Down:
		Rotator.Pitch -= 90.0f;
		break;
	case EMovementDirection::Left:
		Rotator.Yaw -= 90.0f;
		break;
	case EMovementDirection::Right:
		Rotator.Yaw += 90.0f;
		break;
	case EMovementDirection::Forward:
		break;
	}

	return Rotator;
}

ASnakeBaseElement* ASnakeBase::Component2Element(const UChildActorComponent* Component)
{
	return Cast<ASnakeBaseElement>(Component->GetChildActor());
}

ECollisionEnabled::Type ASnakeBase::ToggleCollision(ECollisionEnabled::Type NewCollisionType)
{
	if (NewCollisionType != CollisionState)
	{
		for (const auto It : SnakeComponents)
			Component2Element(It)->GetComponent()->SetCollisionEnabled(NewCollisionType);
		const auto PreviousState = CollisionState;
		CollisionState = NewCollisionType;
		return PreviousState;
	}
	return CollisionState;
}

UChildActorComponent* ASnakeBase::GrowSnake()
{
	const auto IsHead = SnakeComponents.Num() == 0;

	FVector Location = FVector::ZeroVector;

	const auto Component = NewObject<UChildActorComponent>(this);
	Component->RegisterComponent();

	Component->SetChildActorClass(ASnakeBaseElement::StaticClass());
	Component->CreateChildActor();

	const auto Mesh = IsHead ? HeadStaticMesh : BodyStaticMesh;

	Component2Element(Component)->Initialize(this, Mesh, ECollisionEnabled::NoCollision);

	Component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (!IsHead)
	{
		const auto TailActor = Component2Element(GetTail());
		const auto GrownActor = Component2Element(Component);

		const auto Offset = GrownActor->GetXOffset(SnakeElementSpace) +
			TailActor->GetXOffset(SnakeElementSpace);

		Location = GetTail()->GetRelativeLocation() - Offset;
	}

	SnakeComponents.Add(Component);

	Component->SetRelativeLocation(Location);

	return Component;
}

UChildActorComponent* ASnakeBase::GetHead() const
{
	return SnakeComponents[0];
}

UChildActorComponent* ASnakeBase::GetNeck() const
{
	return SnakeComponents[1];
}

UChildActorComponent* ASnakeBase::GetTail() const
{
	return SnakeComponents.Last();
}

void ASnakeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (HeadStaticMesh == nullptr || BodyStaticMesh == nullptr)
		return;

	SnakeComponents.Empty(SnakeInitialSize);

	while (SnakeComponents.Num() < SnakeInitialSize)
		GrowSnake()->CreationMethod = EComponentCreationMethod::UserConstructionScript;
}

void ASnakeBase::MoveSnake(EMovementDirection Direction)
{
	const auto PreviousCollisionState = ToggleCollision(ECollisionEnabled::NoCollision);

	const auto Rotator = Direction2Rotator(Direction);

	for (int i = SnakeComponents.Num() - 1; i > 0; i--)
	{
		const auto PreviousTransform = SnakeComponents[i - 1]->GetRelativeTransform();
		SnakeComponents[i]->SetRelativeTransform(PreviousTransform);
	}

	GetHead()->AddLocalRotation(Rotator);

	const auto Offset = Component2Element(GetHead())->GetXOffset(SnakeElementSpace) +
		Component2Element(GetNeck())->GetXOffset(SnakeElementSpace);

	GetHead()->AddLocalOffset(Offset);

	PendingDirection = EMovementDirection::Forward;

	ToggleCollision(PreviousCollisionState);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick disabled in begin play if no components so snake is valid and have at least one component

	if (IsGrowPending)
	{
		IsGrowPending = false;
		GrowSnake();
	}

	MoveSnake(PendingDirection);
}

void ASnakeBase::SetDirection(EMovementDirection Direction)
{
	PendingDirection = Direction;
}

void ASnakeBase::HrumHrum(AActor* Whom)
{
	IsGrowPending = true;
	Whom->Destroy();
}

void ASnakeBase::HandleCollision(
	ASnakeBaseElement* SnakeElement,
	UPrimitiveComponent* SnakeComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision!!!"));

	if (SnakeElement != Component2Element(GetHead()))
		return;

	if (OtherActor == Component2Element(GetNeck()))
		return;

	if (Cast<AApple>(OtherActor) != nullptr)
	{
		HrumHrum(OtherActor);
		ASnakeGameModeBase::Get()->SpawnApple();
		return;
	}

	ASnakeGameModeBase::Get()->GameOver();
}
