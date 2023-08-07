// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourComponent.h"
#include "GameOverComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API UGameOverComponent : public UBehaviourComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameOverComponent();

	virtual void Affect(AMrsSnakeBase* Snake) const override;
};
