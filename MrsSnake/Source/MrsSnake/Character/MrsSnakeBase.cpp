// Fill out your copyright notice in the Description page of Project Settings.


#include "MrsSnakeBase.h"
#include "MrsSnakeElement.h"
#include "MrsSnake/Components/BehaviourComponent.h"
#include "MrsSnake/Game/MrsSnakeGameModeBase.h"
#include "MrsSnake/Game/MrsSnakePlayerPawnBase.h"
#include "MrsSnake/Misc/ItemBase.h"

// Sets default values
AMrsSnakeBase::AMrsSnakeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMrsSnakeBase::RequestGrow() { IsGrowPending = true; }

void AMrsSnakeBase::IncreaseSpeed(float Percent)
{
	const auto CurrentTickInterval = GetActorTickInterval();
	const auto NewTickInterval = FMath::Max(CurrentTickInterval * (1.0f - Percent), MinimalTickInterval);
	UE_LOG(LogTemp, Log, TEXT("NewTickInterval = %f"), NewTickInterval);
	SetActorTickInterval(NewTickInterval);
}

void AMrsSnakeBase::IncreaseLife(float Delta)
{
	LifeTimeRemain += Delta;
	if (LifeTimeRemain > LifeTimeStart)
		LifeTimeRemain = LifeTimeStart;
}

void AMrsSnakeBase::ReleaseSnake()
{
	SetActorTickInterval(BeginTickInterval);
	SetActorTickEnabled(true);

	const auto* GameMode = AMrsSnakeGameModeBase::Get();
	GameMode->GetTimerManager().SetTimer(
		LifeTimerHandle,
		this,
		&AMrsSnakeBase::LifeTimerTick,
		LifeTimeTick,
		true);
}

void AMrsSnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AMrsSnakeBase::LifeTimerTick()
{
	LifeTimeRemain -= LifeTimeTick;
}

bool AMrsSnakeBase::IsSnakeValid() const
{
	return SnakeComponents.Num() > 1;
}

FRotator AMrsSnakeBase::Direction2Rotator(EMovementDirection Direction)
{
	FRotator Rotator = {0.0f, 0.0f, 0.0f};

	switch (Direction)
	{
	case EMovementDirection::Up:
		Rotator.Yaw = 0.0f;
		break;
	case EMovementDirection::Down:
		Rotator.Yaw = 180.0f;
		break;
	case EMovementDirection::Left:
		Rotator.Yaw = -90.0f;
		break;
	case EMovementDirection::Right:
		Rotator.Yaw = 90.0f;
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
		const auto Offset = FVector{ElementSpace, 0.0f, 0.0f};
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

	SnakeComponents.Empty(InitialSize);

	while (SnakeComponents.Num() < InitialSize)
		GrowSnake()->CreationMethod = EComponentCreationMethod::UserConstructionScript;
}

void AMrsSnakeBase::Destroyed()
{
	Super::Destroyed();

	const auto* GameMode = AMrsSnakeGameModeBase::Get();
	GameMode->GetTimerManager().ClearTimer(LifeTimerHandle);
}

void AMrsSnakeBase::MoveSnake()
{
	const auto PreviousCollisionState = ToggleCollision(ECollisionEnabled::NoCollision);

	for (int i = SnakeComponents.Num() - 1; i > 0; i--)
	{
		const auto PreviousTransform = SnakeComponents[i - 1]->GetRelativeTransform();
		SnakeComponents[i]->SetRelativeTransform(PreviousTransform);
	}

	const auto Rotator = Direction2Rotator(PendingDirection);
	CurrentDirection = PendingDirection;

	GetHead()->SetWorldRotation(Rotator);

	const auto Offset = FVector{ElementSpace, 0.0f, 0.0f};

	GetHead()->AddLocalOffset(Offset);

	for (const auto It : SnakeComponents)
		It->SetWorldLocation(It->GetComponentLocation().GridSnap(ElementSpace / 2.0f));

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

	MoveSnake();

	if (IsDiePending)
		Destroy();
}

void AMrsSnakeBase::SetDirection(EMovementDirection NewDirection)
{
	if (abs(static_cast<int>(CurrentDirection) - static_cast<int>(NewDirection)) != 2)
		PendingDirection = NewDirection;
}

void AMrsSnakeBase::HandleCollision(
	AMrsSnakeElement* SnakeElement,
	UPrimitiveComponent* SnakeComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent)
{
	if (SnakeElement != Component2Element(GetHead()))
		return;

	TArray<UBehaviourComponent*> BehaviourComponents = {};
	OtherActor->GetComponents(BehaviourComponents);

	for (const auto It : BehaviourComponents)
		It->Affect(this);
}
