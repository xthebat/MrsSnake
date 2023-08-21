// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverComponent.h"

#include "MrsSnake/Character/MrsSnakeBase.h"
#include "MrsSnake/Game/MrsSnakeGameModeBase.h"

void UGameOverComponent::Affect(AMrsSnakeBase* Snake) const
{
	Snake->Kill();
}
