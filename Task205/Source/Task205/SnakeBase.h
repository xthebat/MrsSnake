// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakePlayerPawn.h"
#include "GameFramework/Actor.h"
#include "SnakeBase.generated.h"

class ASnakeBaseElement;

UCLASS()
class TASK205_API ASnakeBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(EditAnywhere)
	int SnakeInitialSize = 5;

	UPROPERTY(EditAnywhere)
	float SnakeElementSpace = 10.0;

	UPROPERTY(EditAnywhere)
	float SnakeTickTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* HeadStaticMesh = {};

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* BodyStaticMesh = {};

	// UPROPERTY(EditDefaultsOnly)
	// TSubclassOf<ASnakeBaseElement> SnakeBaseElementClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	TArray<UChildActorComponent*> SnakeComponents = {};

	static FRotator Direction2Rotator(EMovementDirection Direction);

	static ASnakeBaseElement* Component2Element(const UChildActorComponent* Component);

	ECollisionEnabled::Type ToggleCollision(ECollisionEnabled::Type NewCollisionType);

	UChildActorComponent* GrowSnake();

	UChildActorComponent* GetHead() const;
	UChildActorComponent* GetNeck() const;
	UChildActorComponent* GetTail() const;

	void MoveSnake(EMovementDirection Direction);

	bool IsGrowPending = false;

	ECollisionEnabled::Type CollisionState = ECollisionEnabled::QueryOnly;

	EMovementDirection PendingDirection = EMovementDirection::Forward;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDirection(EMovementDirection Direction);
	void HrumHrum(AActor* Whom);

	void HandleCollision(
		ASnakeBaseElement* SnakeElement,
		UPrimitiveComponent* SnakeComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent);
};
