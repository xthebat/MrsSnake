// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourComponent.h"
#include "SpeedupComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API USpeedupComponent : public UBehaviourComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChangeTickPercent = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChangeLifeRemain = 5.0f;

	virtual void Affect(AMrsSnakeBase* Snake) const override;
};