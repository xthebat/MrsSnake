// Fill out your copyright notice in the Description page of Project Settings.


#include "GrowComponent.h"
#include "MrsSnake/Character/MrsSnakeBase.h"
#include "MrsSnake/Game/MrsSnakeGameModeBase.h"

void UGrowComponent::Affect(AMrsSnakeBase* Snake) const
{
	Snake->RequestGrow();
}

