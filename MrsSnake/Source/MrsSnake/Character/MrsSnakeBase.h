// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MrsSnake/Game/MrsSnakePlayerPawnBase.h"
#include "MrsSnake/Widget/MrsSnakeInfoBase.h"
#include "MrsSnakeBase.generated.h"

class AMrsSnakeElement;

UCLASS()
class MRSSNAKE_API AMrsSnakeBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMrsSnakeBase();

	void RequestGrow();

	void IncreaseSpeed(float Percent);
	void IncreaseLife(float Delta);
	void IncreaseScore(int Amount);

	float GetElementSpace() const { return ElementSpace; }
	float GetLifeTimeStart() const { return LifeTimeStart; }
	float GetLifeTimeRemain() const { return LifeTimeRemain; }
	int GetScore() const { return Score; }

	void ReleaseSnake();
	void Kill() { IsDiePending = true; }

protected:
	constexpr static int MaxScore = 2000;

	virtual void BeginPlay() override;

	void LifeTimerTick();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* HeadStaticMesh = {};

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* BodyStaticMesh = {};

	UPROPERTY(EditAnywhere)
	int InitialSize = 3;

	UPROPERTY(EditAnywhere)
	float ElementSpace = 100.0;

	UPROPERTY(EditAnywhere)
	float BeginTickInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float MinimalTickInterval = 0.005f;

	FTimerHandle LifeTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	float LifeTimeStart = 60.0f;

	UPROPERTY(EditDefaultsOnly)
	float LifeTimeTick = 1.0f;

	float LifeTimeRemain = LifeTimeStart;

	int Score = 0;

	UPROPERTY()
	TArray<UChildActorComponent*> SnakeComponents = {};

	static FRotator Direction2Rotator(EMovementDirection Direction);

	static AMrsSnakeElement* Component2Element(const UChildActorComponent* Component);

	ECollisionEnabled::Type ToggleCollision(ECollisionEnabled::Type NewCollisionType);

	UFUNCTION()
	UChildActorComponent* GrowSnake();

	void MoveSnake();

	bool IsGrowPending = false;

	bool IsDiePending = false;

	ECollisionEnabled::Type CollisionState = ECollisionEnabled::QueryOnly;

	EMovementDirection CurrentDirection = {};
	EMovementDirection PendingDirection = {};

public:
	UChildActorComponent* GetHead() const;
	UChildActorComponent* GetTail() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDirection(EMovementDirection NewDirection);

	void HandleCollision(
		AMrsSnakeElement* SnakeElement,
		UPrimitiveComponent* SnakeComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent);

	bool IsSnakeValid() const;
};
