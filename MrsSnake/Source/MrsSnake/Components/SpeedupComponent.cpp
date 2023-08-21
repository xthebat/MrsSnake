// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedupComponent.h"

#include "MrsSnake/Character/MrsSnakeBase.h"

void USpeedupComponent::Affect(AMrsSnakeBase* Snake) const
{
	Snake->IncreaseSpeed(ChangeTickPercent);
	Snake->IncreaseLife(ChangeLifeRemain);
}
