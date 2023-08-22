// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UBehaviourComponent;
class UWidgetComponent;

UCLASS()
class MRSSNAKE_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();

	bool CanGrowSnake() const;

	TArray<UBehaviourComponent*> GetBehaviourComponents() const;

	bool GetIsFloor() const { return IsFloor; }

#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyAboutToChange);
#endif

protected:
	constexpr static float DefaultSelfDestructionTick = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	bool IsFloor = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!IsFloor", EditConditionHides))
	bool IsSelfDestructible = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!IsFloor", EditConditionHides))
	bool IsRandomizeSelfDestructionTime = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "IsSelfDestructible", EditConditionHides))
	float SelfDestructionTime = -1.0f;

	UPROPERTY(EditDefaultsOnly)
	bool IsShowHealthRemain = true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle SelfDestructionHandle;

	float SelfDestructionRemain = {};

	float SelfDestructionTick = {};

	float GetSelfDestructionTick() const;

	UFUNCTION()
	void OnSelfDestructionTick();

public:
	UFUNCTION(BlueprintCallable)
	static AMrsSnakeGameModeBase* GetSnakeGameMode();

	UFUNCTION(BlueprintImplementableEvent)
	void ConfigureItem();

	void ConfigureItem_Implementation();
};
