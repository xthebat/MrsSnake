// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MrsSnake/Components/BehaviourComponent.h"
#include "MrsSnakeGameModeBase.generated.h"

class ISpawner;
class UMrsSnakeInfoBase;
class AItemBase;
class AApple;

USTRUCT(BlueprintType)
struct FItemDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemBase> ItemClass;

	UPROPERTY(EditAnywhere)
	float Chance = 0.5f;

	UPROPERTY(EditAnywhere)
	bool IsEnabled = true;

	UPROPERTY(EditAnywhere)
	bool IsGrowItem = false;

	UPROPERTY(EditAnywhere)
	bool IsBlockItem = false;

	UPROPERTY(EditAnywhere, meta=(MustImplement="Spawner"))
	TSubclassOf<UObject> SpawnerClass;

	UPROPERTY(EditAnywhere)
	TMap<FString, FString> SpawnerParameters; // Why no FVariant .... ???
};

/**
 * 
 */
UCLASS()
class MRSSNAKE_API AMrsSnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMrsSnakeGameModeBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void StartPlay() override;

	static UWorld* GetMyWorld();
	static APlayerController* GetPlayer(int Index = 0);
	static AHUD* GetHUD(int Index);
	static AMrsSnakeGameModeBase* Get();
	static FTimerManager& GetTimerManager();

	AActor* DoSpawnItem(const TSubclassOf<AItemBase> ItemClass, FVector Location);
	bool CanSpawnItem(FVector Location) const;

	template <class Cls>
	static Cls* GetActorOfClass()
	{
		return Cast<Cls>(UGameplayStatics::GetActorOfClass(GetMyWorld(), Cls::StaticClass()));
	}

	template <class Cls>
	static TArray<Cls*> GetAllActorsOfClass()
	{
		TArray<AActor*> Actors = {};
		UGameplayStatics::GetAllActorsOfClass(GetMyWorld(), Cls::StaticClass(), Actors);
		return reinterpret_cast<TArray<Cls*>&>(Actors);
	}

	UFUNCTION(BlueprintCallable)
	static void QuitGame();

	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FItemDescription> ItemsDescriptions;

	UPROPERTY(EditDefaultsOnly)
	int SpawnTries = 10;

	UPROPERTY(EditDefaultsOnly)
	int NearestSpawnLocation = 3;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMrsSnakeInfoBase> MrsSnakeInfoClass;

	UPROPERTY()
	AMrsSnakeBase* MrsSnake = nullptr;

	UPROPERTY()
	AItemBase* Floor = nullptr;

	UPROPERTY()
	UMrsSnakeInfoBase* Info = nullptr;

	FVector BottomLeft = {};
	FVector TopRight = {};
	float Grid = {};

	UPROPERTY()
	TMap<FVector, AActor*> SceneActors = {};

	UPROPERTY()
	AActor* GrowItem = nullptr;

	FItemDescription* AppleDescription = nullptr;

	FItemDescription* BlockDescription = nullptr;

	bool IsGameStarted = false;

	bool CanEverStart = false;

	bool GameOver = false;

	bool SpawnItem(const FItemDescription* ItemDescription);

	UFUNCTION()
	void OnMrsSnakeDie(AActor* Actor);

	UFUNCTION()
	void OnSceneActorDestroyed(AActor* Actor);

	// Configurable parameters functions

	int ChangeTickPercent = 1;

public:
	UFUNCTION(BlueprintCallable)
	int GetChangeTickPercent() const { return ChangeTickPercent; }

	UFUNCTION(BlueprintCallable)
	void SetChangeTickPercent(int NewChangeTickPercent) { ChangeTickPercent = NewChangeTickPercent; }
};
