// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourComponent.h"
#include "GrowComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API UGrowComponent : public UBehaviourComponent
{
	GENERATED_BODY()

public:
	virtual void Affect(AMrsSnakeBase* Snake) const override;
};