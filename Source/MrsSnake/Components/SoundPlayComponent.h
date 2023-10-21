// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourComponent.h"
#include "SoundPlayComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API USoundPlayComponent : public UBehaviourComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	USoundBase* Sound = nullptr;
public:
	virtual void Affect(AMrsSnakeBase* Snake) const override;
};
