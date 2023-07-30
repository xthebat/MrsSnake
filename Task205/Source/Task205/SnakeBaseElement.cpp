// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBaseElement.h"
#include "SnakeBase.h"

// Sets default values
ASnakeBaseElement::ASnakeBaseElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	const auto StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootStaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionResponseToChannels(ECR_Overlap);
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ASnakeBaseElement::BeginPlay()
{
	Super::BeginPlay();
	GetComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASnakeBaseElement::HandleBeginOverlap);
}

void ASnakeBaseElement::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Snake->HandleBeginOverlap(this, OverlappedComponent, OtherActor, OtherComponent);
}

UStaticMeshComponent* ASnakeBaseElement::GetComponent() const
{
	return Cast<UStaticMeshComponent>(RootComponent);
}

void ASnakeBaseElement::Initialize(ASnakeBase* Parent, UStaticMesh* Mesh, ECollisionEnabled::Type CollisionType)
{
	GetComponent()->SetStaticMesh(Mesh);
	GetComponent()->SetCollisionEnabled(CollisionType);
	Snake = Parent;
}

UStaticMesh* ASnakeBaseElement::GetStaticMesh() const
{
	return GetComponent()->GetStaticMesh();
}

FVector ASnakeBaseElement::GetXOffset(float Offset) const
{
	const auto Mesh = GetStaticMesh();
	const auto Box = Mesh->GetBoundingBox();
	const auto Extents = Box.GetExtent() - Box.GetCenter();
	return (Extents + Offset) * FVector::ForwardVector;
}

// Called every frame
void ASnakeBaseElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
