// Copyright Epic Games, Inc. All Rights Reserved.


#include "MrsSnakeGameModeBase.h"

#include "Apple.h"
#include "Kismet/GameplayStatics.h"


void AMrsSnakeGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMrsSnakeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnApple();
}

UWorld* AMrsSnakeGameModeBase::GetWorld()
{
	return GEngine->GameViewport->GetWorld();
}

APlayerController* AMrsSnakeGameModeBase::GetPlayer(int Index)
{
	return UGameplayStatics::GetPlayerController(GetWorld(), Index);
}

AHUD* AMrsSnakeGameModeBase::GetHUD(int Index)
{
	return GetPlayer(Index)->GetHUD();
}

AMrsSnakeGameModeBase* AMrsSnakeGameModeBase::Get()
{
	return Cast<AMrsSnakeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

FTimerManager& AMrsSnakeGameModeBase::GetTimerManager()
{
	return GetWorld()->GetTimerManager();
}

void AMrsSnakeGameModeBase::SpawnApple() const
{
	if (!IsValid(AppleClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Apple class was not specified, not apple will be spawned :("));
		return;
	}

	const auto Apple = GetActorOfClass<AApple>();
	if (Apple == nullptr)
	{
		const auto Location = FVector{
			FMath::FRandRange(-650, 650),
			FMath::FRandRange(-850, 850),
			FMath::FRandRange(0, 0),
		}.GridSnap(100);
		const auto Rotation = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AApple>(AppleClass, Location, Rotation);
	}
}

void AMrsSnakeGameModeBase::GameOver()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetPlayer(), EQuitPreference::Quit, false);
}
