// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MrsSnake/Game/MrsSnakePlayerPawnBase.h"
#include "MrsSnakeBase.generated.h"

class AMrsSnakeElement;

UCLASS()
class MRSSNAKE_API AMrsSnakeBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMrsSnakeBase();

	UPROPERTY(EditAnywhere)
	int SnakeInitialSize = 5;

	UPROPERTY(EditAnywhere)
	float SnakeElementSpace = 100.0;

	UPROPERTY(EditAnywhere)
	float SnakeTickTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* HeadStaticMesh = {};

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* BodyStaticMesh = {};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	TArray<UChildActorComponent*> SnakeComponents = {};

	static FRotator Direction2Rotator(EMovementDirection Direction);

	static AMrsSnakeElement* Component2Element(const UChildActorComponent* Component);

	ECollisionEnabled::Type ToggleCollision(ECollisionEnabled::Type NewCollisionType);

	UChildActorComponent* GetHead() const;
	UChildActorComponent* GetTail() const;

	UChildActorComponent* GrowSnake();

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
		AMrsSnakeElement* SnakeElement,
		UPrimitiveComponent* SnakeComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent);
};
