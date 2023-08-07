// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/WidgetComponent.h"
#include "MrsSnake/Game/MrsSnakeGameModeBase.h"
#include "MrsSnake/Widget/ItemInfo.h"

// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

float AItemBase::GetSelfDestructionTick() const
{
	if (SelfDestructionRemain >= 1.0f)
		return 1.0f;

	return SelfDestructionRemain;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	SelfDestructionRemain = SelfDestructionTime;
	SelfDestructionTick = GetSelfDestructionTick();

	const auto Component = Cast<UWidgetComponent>(this->GetComponentByClass(UWidgetComponent::StaticClass()));
	if (Component != nullptr)
	{
		const auto Widget = Cast<UItemInfo>(Component->GetWidget());
		if (Widget != nullptr)
		{
			Widget->Health = SelfDestructionRemain;
			Widget->MaxHealth = SelfDestructionRemain;
		}
	}

	const auto* GameMode = AMrsSnakeGameModeBase::Get();
	GameMode->GetTimerManager().SetTimer(
		SelfDestructionHandle,
		this,
		&AItemBase::OnSelfDestructionTick,
		SelfDestructionTick,
		false);
}

void AItemBase::OnSelfDestructionTick()
{
	SelfDestructionRemain -= SelfDestructionTick;

	const auto Component = Cast<UWidgetComponent>(this->GetComponentByClass(UWidgetComponent::StaticClass()));
	if (Component != nullptr)
	{
		const auto Widget = Cast<UItemInfo>(Component->GetWidget());
		if (Widget != nullptr)
			Widget->Health = SelfDestructionRemain;
	}

	if (SelfDestructionRemain == 0.0f)
	{
		Destroy();
		return;
	}

	SelfDestructionTick = GetSelfDestructionTick();

	const auto* GameMode = AMrsSnakeGameModeBase::Get();
	GameMode->GetTimerManager().SetTimer(
		SelfDestructionHandle,
		this,
		&AItemBase::OnSelfDestructionTick,
		SelfDestructionTick,
		false);
}
