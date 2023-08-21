// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviourComponent.generated.h"

class AMrsSnakeGameModeBase;
class AMrsSnakeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConfigureBehaviourComponent);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MRSSNAKE_API UBehaviourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBehaviourComponent();

	virtual void Affect(AMrsSnakeBase* Snake) const;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FConfigureBehaviourComponent ConfigureBehaviourComponent;
};
