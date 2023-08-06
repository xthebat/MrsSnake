// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	static AMrsSnakeGameModeBase* Get();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AApple> AppleClass;

	void SpawnApple() const;

	static void GameOver();
};
