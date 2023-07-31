// Copyright Epic Games, Inc. All Rights Reserved.


#include "SnakeGameModeBase.h"

#include "Apple.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(SnakeLog);

void ASnakeGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASnakeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnApple();
}

UWorld* ASnakeGameModeBase::GetWorld()
{
	return GEngine->GameViewport->GetWorld();
}

APlayerController* ASnakeGameModeBase::GetPlayer()
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

ASnakeGameModeBase* ASnakeGameModeBase::Get()
{
	return Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ASnakeGameModeBase::SpawnApple()
{
	if (!IsValid(AppleClass))
	{
		UE_LOG(SnakeLog, Warning, TEXT("Apple class was not specified, not apple will be spawned :("));
		return;
	}

	const auto Location = FVector{
		FMath::FRandRange(-650, 650),
		FMath::FRandRange(-850, 850),
		FMath::FRandRange(0, 500),
	};
	const auto Rotation = FRotator::ZeroRotator;
	GetWorld()->SpawnActor<AApple>(AppleClass, Location, Rotation);
}

void ASnakeGameModeBase::GameOver()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetPlayer(), EQuitPreference::Quit, false);
}
