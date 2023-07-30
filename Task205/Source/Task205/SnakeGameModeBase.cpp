// Copyright Epic Games, Inc. All Rights Reserved.


#include "SnakeGameModeBase.h"

#include "Apple.h"
#include "Kismet/GameplayStatics.h"

void ASnakeGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

ASnakeGameModeBase* ASnakeGameModeBase::GetFor(const AActor* Context)
{
	return Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(Context->GetWorld()));
}

void ASnakeGameModeBase::SpawnApple()
{
	const auto Location = FVector{
		FMath::FRandRange(-500, 500),
		FMath::FRandRange(-500, 500),
		FMath::FRandRange(0, 100),
	};
	const auto Rotation = FRotator::ZeroRotator;
	GetWorld()->SpawnActor<AApple>(AppleClass, Location, Rotation);
}

void ASnakeGameModeBase::GameOver(AActor* Whom)
{
	RequestEngineExit(TEXT("YOU ARE LOSE"));
}
