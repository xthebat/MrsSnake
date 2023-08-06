// Fill out your copyright notice in the Description page of Project Settings.


#include "MrsSnakeBase.h"

#include "MrsSnakeBaseElement.h"
#include "MrsSnakeGameModeBase.h"
#include "Apple.h"

// Sets default values
AMrsSnakeBase::AMrsSnakeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMrsSnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(SnakeTickTime);
	SetActorTickEnabled(SnakeComponents.Num() > 1); // Disable ticks if snake is invalid
}

FRotator AMrsSnakeBase::Direction2Rotator(EMovementDirection Direction)
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

AMrsSnakeBaseElement* AMrsSnakeBase::Component2Element(const UChildActorComponent* Component)
{
	return Cast<AMrsSnakeBaseElement>(Component->GetChildActor());
}

ECollisionEnabled::Type AMrsSnakeBase::ToggleCollision(ECollisionEnabled::Type NewCollisionType)
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

UChildActorComponent* AMrsSnakeBase::GrowSnake()
{
	const auto IsHead = SnakeComponents.Num() == 0;

	FVector Location = FVector::ZeroVector;

	const auto Component = NewObject<UChildActorComponent>(this);
	Component->RegisterComponent();

	Component->SetChildActorClass(AMrsSnakeBaseElement::StaticClass());
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

UChildActorComponent* AMrsSnakeBase::GetHead() const
{
	return SnakeComponents[0];
}

UChildActorComponent* AMrsSnakeBase::GetNeck() const
{
	return SnakeComponents[1];
}

UChildActorComponent* AMrsSnakeBase::GetTail() const
{
	return SnakeComponents.Last();
}

void AMrsSnakeBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (HeadStaticMesh == nullptr || BodyStaticMesh == nullptr)
		return;

	SnakeComponents.Empty(SnakeInitialSize);

	while (SnakeComponents.Num() < SnakeInitialSize)
		GrowSnake()->CreationMethod = EComponentCreationMethod::UserConstructionScript;
}

void AMrsSnakeBase::MoveSnake(EMovementDirection Direction)
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
void AMrsSnakeBase::Tick(float DeltaTime)
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

void AMrsSnakeBase::SetDirection(EMovementDirection Direction)
{
	PendingDirection = Direction;
}

void AMrsSnakeBase::HrumHrum(AActor* Whom)
{
	IsGrowPending = true;
	Whom->Destroy();
}

void AMrsSnakeBase::HandleCollision(
	AMrsSnakeBaseElement* SnakeElement,
	UPrimitiveComponent* SnakeComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent)
{
	if (SnakeElement != Component2Element(GetHead()))
		return;

	if (OtherActor == Component2Element(GetNeck()))
		return;

	if (Cast<AApple>(OtherActor) != nullptr)
	{
		HrumHrum(OtherActor);
		AMrsSnakeGameModeBase::Get()->SpawnApple();
		return;
	}

	AMrsSnakeGameModeBase::Get()->GameOver();
}
