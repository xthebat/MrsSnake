// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameModeBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SnakeLog, Log, All);

class AApple;

/**
 * 
 */
UCLASS()
class TASK205_API ASnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	static UWorld* GetWorld();
	static APlayerController* GetPlayer();
	static ASnakeGameModeBase* Get();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AApple> AppleClass;

	void SpawnApple();

	void GameOver();
};
