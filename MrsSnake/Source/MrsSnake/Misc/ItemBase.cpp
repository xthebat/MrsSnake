// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/WidgetComponent.h"
#include "MrsSnake/Components/BehaviourComponent.h"
#include "MrsSnake/Components/GrowComponent.h"
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
	if (SelfDestructionRemain >= DefaultSelfDestructionTick)
		return DefaultSelfDestructionTick;

	return SelfDestructionRemain;
}

bool AItemBase::CanGrowSnake() const
{
	return GetBehaviourComponents().FindItemByClass<UGrowComponent>();
}

TArray<UBehaviourComponent*> AItemBase::GetBehaviourComponents() const
{
	TArray<UBehaviourComponent*> BehaviourComponents = {};
	GetComponents(BehaviourComponents);
	return BehaviourComponents;
}


// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (!IsSelfDestructible || SelfDestructionTime < 0)
		return;

	if (IsRandomizeSelfDestructionTime)
		SelfDestructionTime = FMath::GridSnap(
			FMath::RandRange(DefaultSelfDestructionTick, SelfDestructionTime),
			DefaultSelfDestructionTick
		);

	SelfDestructionRemain = SelfDestructionTime;
	SelfDestructionTick = GetSelfDestructionTick();

	const auto* GameMode = AMrsSnakeGameModeBase::Get();
	GameMode->GetTimerManager().SetTimer(
		SelfDestructionHandle,
		this,
		&AItemBase::OnSelfDestructionTick,
		SelfDestructionTick,
		false);

	const auto WidgetComponent = Cast<UWidgetComponent>(this->GetComponentByClass(UWidgetComponent::StaticClass()));

	if (WidgetComponent == nullptr)
		return;

	if (!IsShowHealthRemain)
		WidgetComponent->SetWidget(nullptr);

	const auto Widget = Cast<UItemInfo>(WidgetComponent->GetWidget());
	if (Widget != nullptr)
	{
		Widget->Health = SelfDestructionRemain;
		Widget->MaxHealth = SelfDestructionRemain;
	}
}

void AItemBase::OnSelfDestructionTick()
{
	if (!IsValid(this))
		return;

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

#if WITH_EDITOR
void AItemBase::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);

	if (PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(AItemBase, IsFloor))
		IsSelfDestructible = false;

	if (PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(AItemBase, IsSelfDestructible))
	{
		SelfDestructionTime = -1.0f;
		IsRandomizeSelfDestructionTime = false;
	}
}
#endif
