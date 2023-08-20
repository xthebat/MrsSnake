// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourComponent.h"
#include "DestroyableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API UDestroyableComponent : public UBehaviourComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDestroyableComponent();

	virtual void Affect(AMrsSnakeBase* Snake) const override;
};
