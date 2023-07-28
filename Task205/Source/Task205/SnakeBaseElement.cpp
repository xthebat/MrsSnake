// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBaseElement.h"

// Sets default values
ASnakeBaseElement::ASnakeBaseElement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootStaticMesh"));
}

// Called when the game starts or when spawned
void ASnakeBaseElement::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnakeBaseElement::SetStaticMesh(UStaticMesh* Mesh) const
{
	Cast<UStaticMeshComponent>(RootComponent)->SetStaticMesh(Mesh);
}

// Called every frame
void ASnakeBaseElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

