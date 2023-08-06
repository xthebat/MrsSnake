// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "ItemInfo.h"
#include "Components/WidgetComponent.h"
#include "MrsSnakeGameModeBase.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

float AItem::GetSelfDestructionTick() const
{
	if (SelfDestructionRemain >= 1.0f)
		return 1.0f;

	return SelfDestructionRemain;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
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
		&AItem::OnSelfDestructionTick,
		SelfDestructionTick,
		false);
}

void AItem::OnSelfDestructionTick()
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
		&AItem::OnSelfDestructionTick,
		SelfDestructionTick,
		false);
}
