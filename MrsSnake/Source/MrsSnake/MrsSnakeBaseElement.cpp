// Fill out your copyright notice in the Description page of Project Settings.


#include "MrsSnakeBaseElement.h"
#include "MrsSnakeBase.h"

// Sets default values
AMrsSnakeBaseElement::AMrsSnakeBaseElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	const auto StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootStaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionResponseToChannels(ECR_Overlap);
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AMrsSnakeBaseElement::BeginPlay()
{
	Super::BeginPlay();

	GetComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMrsSnakeBaseElement::HandleBeginOverlap);
}

void AMrsSnakeBaseElement::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Snake->HandleCollision(this, OverlappedComponent, OtherActor, OtherComponent);
}

UStaticMeshComponent* AMrsSnakeBaseElement::GetComponent() const
{
	return Cast<UStaticMeshComponent>(RootComponent);
}

void AMrsSnakeBaseElement::Initialize(AMrsSnakeBase* Parent, UStaticMesh* Mesh, ECollisionEnabled::Type CollisionType)
{
	GetComponent()->SetStaticMesh(Mesh);
	GetComponent()->SetCollisionEnabled(CollisionType);
	Snake = Parent;
}

UStaticMesh* AMrsSnakeBaseElement::GetStaticMesh() const
{
	return GetComponent()->GetStaticMesh();
}

FVector AMrsSnakeBaseElement::GetXOffset(float Offset) const
{
	const auto Mesh = GetStaticMesh();
	const auto BodySetup = Mesh->GetBodySetup();
	const auto Sphere = BodySetup->AggGeom.SphereElems[0];
	UE_LOG(LogTemp, Warning, TEXT("Extents: %f %s %s"), Sphere.Radius, *Sphere.Center.ToString(), *Mesh->GetName());
	return FVector{Sphere.Radius - Sphere.Center.X + Offset, 0.0f, 0.0f};
}

// Called every frame
void AMrsSnakeBaseElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
