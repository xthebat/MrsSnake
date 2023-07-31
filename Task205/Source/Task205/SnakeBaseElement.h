// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeBaseElement.generated.h"

class ASnakeBase;

UCLASS()
class TASK205_API ASnakeBaseElement : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASnakeBaseElement();

	void Initialize(ASnakeBase* Parent, UStaticMesh* Mesh, ECollisionEnabled::Type CollisionType);

	UStaticMeshComponent* GetComponent() const;

	UStaticMesh* GetStaticMesh() const;

	FVector GetXOffset(float Offset = 0.0f) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	           FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HandleBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent*
		OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY()
	ASnakeBase* Snake = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
