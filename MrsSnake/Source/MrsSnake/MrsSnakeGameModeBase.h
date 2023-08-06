// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MrsSnakeGameModeBase.generated.h"

class AApple;

/**
 * 
 */
UCLASS()
class MRSSNAKE_API AMrsSnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	static UWorld* GetWorld();
	static APlayerController* GetPlayer(int Index = 0);
	static AHUD* GetHUD(int Index);
	static AMrsSnakeGameModeBase* Get();
	static FTimerManager& GetTimerManager();

	template <class Cls>
	static Cls* GetActorOfClass()
	{
		return Cast<Cls>(UGameplayStatics::GetActorOfClass(GetWorld(), Cls::StaticClass()));
	}

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> AppleClass;

	void SpawnApple() const;

	static void GameOver();
};
