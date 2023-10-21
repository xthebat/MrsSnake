// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyableComponent.h"

void UDestroyableComponent::Affect(AMrsSnakeBase* Snake) const
{
	GetOwner()->Destroy();
}



