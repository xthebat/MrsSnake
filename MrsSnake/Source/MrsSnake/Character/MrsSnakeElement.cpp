// Fill out your copyright notice in the Description page of Project Settings.


#include "MrsSnakeElement.h"
#include "MrsSnakeBase.h"
#include "MrsSnake/Components/GameOverComponent.h"

// Sets default values
AMrsSnakeElement::AMrsSnakeElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	const auto StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootStaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionResponseToChannels(ECR_Overlap);
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AMrsSnakeElement::BeginPlay()
{
	Super::BeginPlay();

	GetComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMrsSnakeElement::HandleBeginOverlap);
}

void AMrsSnakeElement::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Snake->HandleCollision(this, OverlappedComponent, OtherActor, OtherComponent);
}

UStaticMeshComponent* AMrsSnakeElement::GetComponent() const
{
	return Cast<UStaticMeshComponent>(RootComponent);
}

void AMrsSnakeElement::Initialize(AMrsSnakeBase* Parent, UStaticMesh* Mesh, ECollisionEnabled::Type CollisionType)
{
	const auto Component = NewObject<UGameOverComponent>(this);
	Component->RegisterComponent();
	GetComponent()->SetStaticMesh(Mesh);
	GetComponent()->SetCollisionEnabled(CollisionType);
	Snake = Parent;
}

UStaticMesh* AMrsSnakeElement::GetStaticMesh() const
{
	return GetComponent()->GetStaticMesh();
}

// Called every frame
void AMrsSnakeElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
