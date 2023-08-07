// Fill out your copyright notice in the Description page of Project Settings.


#include "MrsSnakeBase.h"
#include "MrsSnakeElement.h"
#include "MrsSnake/Components/BehaviourComponent.h"
#include "MrsSnake/Game/MrsSnakePlayerPawnBase.h"

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

AMrsSnakeElement* AMrsSnakeBase::Component2Element(const UChildActorComponent* Component)
{
	return Cast<AMrsSnakeElement>(Component->GetChildActor());
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

	Component->SetChildActorClass(AMrsSnakeElement::StaticClass());
	Component->CreateChildActor();

	const auto Mesh = IsHead ? HeadStaticMesh : BodyStaticMesh;

	Component2Element(Component)->Initialize(this, Mesh, ECollisionEnabled::NoCollision);

	Component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (!IsHead)
	{
		const auto Offset = FVector{SnakeElementSpace, 0.0f, 0.0f};
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

	const auto Offset = FVector{SnakeElementSpace, 0.0f, 0.0f};

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
	AMrsSnakeElement* SnakeElement,
	UPrimitiveComponent* SnakeComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent)
{
	if (SnakeElement != Component2Element(GetHead()))
		return;

	const auto Component = OtherActor->GetComponentByClass(UBehaviourComponent::StaticClass());
	const auto BehaviourComponent = Cast<UBehaviourComponent>(Component);
	if (BehaviourComponent != nullptr)
		BehaviourComponent->Affect(this);

	// if (Cast<AApple>(OtherActor) != nullptr)
	// {
		// HrumHrum(OtherActor);
		// AMrsSnakeGameModeBase::Get()->SpawnApple();
		// return;
	// }

	// AMrsSnakeGameModeBase::Get()->GameOver();
}
