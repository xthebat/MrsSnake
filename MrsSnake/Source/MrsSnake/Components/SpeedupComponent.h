// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourComponent.h"
#include "SpeedupComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API USpeedupComponent : public UBehaviourComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float ChangeTickInterval = 0.01f;

	UPROPERTY(EditAnywhere)
	float ChangeLifeRemain = 5.0f;
public:
	// Sets default values for this component's properties
	USpeedupComponent();

	virtual void Affect(AMrsSnakeBase* Snake) const override;
};