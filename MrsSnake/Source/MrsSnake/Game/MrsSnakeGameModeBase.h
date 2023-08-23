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

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Generic = 0,
	Food = 1,
	Block = 2,
	Bonus = 3
};

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
	EItemType Type = EItemType::Generic;

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
	static bool HasWorld();
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

	UPROPERTY(EditDefaultsOnly)
	USoundBase* BackgroundMusic = nullptr;

	UPROPERTY()
	UAudioComponent* BackgroundMusicComponent = nullptr;

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

	FItemDescription* BonusDescription = nullptr;

	bool IsGameStarted = false;

	bool CanEverStart = false;

	bool GameOver = false;

	bool SpawnItem(const FItemDescription* ItemDescription);

	UFUNCTION()
	void OnMrsSnakeDie(AActor* Actor);

	UFUNCTION()
	void OnSceneActorDestroyed(AActor* Actor);

	// Configurable parameters functions

	constexpr static int MaxChangeTickPercent = 100;
	constexpr static int MinChangeTickPercent = 1;

	constexpr static int MaxAppleLifeTime = 60;
	constexpr static int MinAppleLifeTime = 10;

	constexpr static int MaxOrangeChance = 100;
	constexpr static int MinOrangeChance = 1;

	constexpr static int MaxBackgroundMusicVolume = 100;
	constexpr static int MinBackgroundMusicVolume = 0;

	int ChangeTickPercent = 5;
	int AppleLifeTime = 10;
	int OrangeChance = 6;
	int BackgroundMusicVolume = 10;

public:
	UFUNCTION(BlueprintCallable)
	int GetChangeTickPercent() const { return ChangeTickPercent; }

	UFUNCTION(BlueprintCallable)
	void SetChangeTickPercent(int Value)
	{
		ChangeTickPercent = FMath::Clamp(Value, MinChangeTickPercent, MaxChangeTickPercent);
	}

	UFUNCTION(BlueprintCallable)
	int GetAppleLifeTime() const { return AppleLifeTime; }

	UFUNCTION(BlueprintCallable)
	void SetAppleLifeTime(int Value)
	{
		AppleLifeTime = FMath::Clamp(Value, MinAppleLifeTime, MaxAppleLifeTime);
	}

	UFUNCTION(BlueprintCallable)
	int GetOrangeChance() const { return OrangeChance; }

	UFUNCTION(BlueprintCallable)
	void SetOrangeChance(int Value)
	{
		OrangeChance = FMath::Clamp(Value, MinOrangeChance, MaxOrangeChance);
	}

	UFUNCTION(BlueprintCallable)
	int GetBackgroundMusicVolume() const { return BackgroundMusicVolume; }

	UFUNCTION(BlueprintCallable)
	void SetBackgroundMusicVolume(int Value)
	{
		BackgroundMusicVolume = FMath::Clamp(Value, MinBackgroundMusicVolume, MaxBackgroundMusicVolume);
	}
};
