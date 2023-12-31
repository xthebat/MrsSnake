﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourComponent.h"
#include "MrsSnake/Game/MrsSnakeGameModeBase.h"

// Sets default values for this component's properties
UBehaviourComponent::UBehaviourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UBehaviourComponent::Affect(AMrsSnakeBase* Snake) const
{

}

void UBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();
	ConfigureBehaviourComponent.Broadcast();
}

