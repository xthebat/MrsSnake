// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MrsSnakeElement.generated.h"

class AMrsSnakeBase;

UCLASS()
class MRSSNAKE_API AMrsSnakeElement : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMrsSnakeElement();

	void Initialize(AMrsSnakeBase* Parent, UStaticMesh* Mesh, ECollisionEnabled::Type CollisionType);

	UStaticMeshComponent* GetComponent() const;

	UStaticMesh* GetStaticMesh() const;

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
	AMrsSnakeBase* Snake = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
