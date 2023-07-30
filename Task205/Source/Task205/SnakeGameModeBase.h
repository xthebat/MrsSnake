// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameModeBase.generated.h"

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

	static ASnakeGameModeBase* GetFor(const AActor* Context);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AApple> AppleClass;

	void SpawnApple();

	void GameOver(AActor* Whom);
};
