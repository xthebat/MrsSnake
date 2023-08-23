// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundPlayComponent.h"

#include "Kismet/GameplayStatics.h"
#include "MrsSnake/Character/MrsSnakeBase.h"


void USoundPlayComponent::Affect(AMrsSnakeBase* Snake) const
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, Snake->GetHead()->GetComponentLocation());
}
