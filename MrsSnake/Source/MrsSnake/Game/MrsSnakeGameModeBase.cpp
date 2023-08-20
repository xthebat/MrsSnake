// Copyright Epic Games, Inc. All Rights Reserved.


#include "MrsSnakeGameModeBase.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "MrsSnake/Character/MrsSnakeBase.h"
#include "MrsSnake/Misc/ItemBase.h"
#include "MrsSnake/Spawner/Spawner.h"
#include "MrsSnake/Widget/MrsSnakeInfo.h"


AMrsSnakeGameModeBase::AMrsSnakeGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMrsSnakeGameModeBase::StartPlay()
{
	Super::StartPlay();
	SetActorTickEnabled(false);

	AppleDescription = ItemsDescriptions.FindByPredicate([](const auto It) { return It.IsGrowItem; });
	if (AppleDescription == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AppleDescription not set!"));
		return;
	}

	BlockDescription = ItemsDescriptions.FindByPredicate([](const auto It) { return It.IsBlockItem; });
	if (BlockDescription == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockDescription not set!"));
		return;
	}

	MrsSnake = GetActorOfClass<AMrsSnakeBase>();
	if (MrsSnake == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MrsSnake not found on level!"));
		return;
	}

	if (!MrsSnake->IsSnakeValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("MrsSnake not valid!"));
		return;
	}

	Floor = *GetAllActorsOfClass<AItemBase>().FindByPredicate([](const auto It) { return It->GetIsFloor(); });
	if (Floor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MrsSnake not found on level!"));
		return;
	}

	FVector Origin = {};
	FVector BoxExtent = {};
	Floor->GetActorBounds(false, Origin, BoxExtent);
	Grid = MrsSnake->GetElementSpace() / 2;

	BottomLeft = Origin + 2.0f * Grid - BoxExtent;
	TopRight = BoxExtent + Origin - 2.0f * Grid;

	UE_LOG(LogTemp, Log, TEXT("BottomLeft = %s TopRight = %s Grid = %f"),
	       *BottomLeft.ToString(),
	       *TopRight.ToString(),
	       Grid);

	const auto HUD = GetHUD(0);
	const auto WidgetComponent = Cast<UWidgetComponent>(HUD->GetComponentByClass(UWidgetComponent::StaticClass()));
	if (WidgetComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MrsSnake WidgetComponent in HUD not found on level!"));
		return;
	}

	Info = Cast<UMrsSnakeInfo>(WidgetComponent->GetUserWidgetObject());
	if (Info == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MrsSnakeInfo not found in: %s %p"), *WidgetComponent->GetName(),
		       WidgetComponent->GetUserWidgetObject());
		return;
	}

	for (float X = Origin.X; X < BoxExtent.X; X += Grid)
	{
		DoSpawnItem(BlockDescription->ItemClass, {X, BoxExtent.Y, 0.0f});
		DoSpawnItem(BlockDescription->ItemClass, {-X, BoxExtent.Y, 0.0f});
		DoSpawnItem(BlockDescription->ItemClass, {X, -BoxExtent.Y, 0.0f});
		DoSpawnItem(BlockDescription->ItemClass, {-X, -BoxExtent.Y, 0.0f});
	}

	for (float Y = Origin.Y; Y < BoxExtent.Y; Y += Grid)
	{
		DoSpawnItem(BlockDescription->ItemClass, {BoxExtent.X, Y, 0.0f});
		DoSpawnItem(BlockDescription->ItemClass, {BoxExtent.X, -Y, 0.0f});
		DoSpawnItem(BlockDescription->ItemClass, {-BoxExtent.X, Y, 0.0f});
		DoSpawnItem(BlockDescription->ItemClass, {-BoxExtent.X, -Y, 0.0f});
	}

	Info->Health = 1.0;
	Info->Message = "Press 'Space' to start the game";

	CanEverStart = true;
}

void AMrsSnakeGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Info->Health = MrsSnake->GetLifeTimeRemain() / MrsSnake->GetLifeTimeStart();

	if (Info->Health == 0)
		QuitGame();

	if (GrowItem == nullptr)
		SpawnItem(AppleDescription);

	const auto ToBeSpawn = ItemsDescriptions.FilterByPredicate([](const auto It)
	{
		return It.IsEnabled && !It.IsGrowItem;
	});

	for (const auto ItemDescription : ToBeSpawn)
	{
		if (SpawnItem(&ItemDescription))
			break;
	}
}

UWorld* AMrsSnakeGameModeBase::GetMyWorld()
{
	return GEngine->GameViewport->GetWorld();
}

APlayerController* AMrsSnakeGameModeBase::GetPlayer(int Index)
{
	return UGameplayStatics::GetPlayerController(GetMyWorld(), Index);
}

AHUD* AMrsSnakeGameModeBase::GetHUD(int Index)
{
	return GetPlayer(Index)->GetHUD();
}

AMrsSnakeGameModeBase* AMrsSnakeGameModeBase::Get()
{
	return Cast<AMrsSnakeGameModeBase>(UGameplayStatics::GetGameMode(GetMyWorld()));
}

FTimerManager& AMrsSnakeGameModeBase::GetTimerManager()
{
	return GetMyWorld()->GetTimerManager();
}

bool AMrsSnakeGameModeBase::CanSpawnItem(FVector Location) const
{
	TArray<AActor*> MrsSnakeElements = {};
	MrsSnake->GetAllChildActors(MrsSnakeElements);

	const auto Found = MrsSnakeElements.FindByPredicate([this, Location](const auto It)
	{
		return It->GetActorLocation().GridSnap(Grid) == Location;
	});

	if (Found != nullptr)
		return false;

	if (SceneActors.Find(Location))
		return false;

	const auto HeadTransform = MrsSnake->GetHead()->GetComponentTransform();
	const auto HeadLocation = HeadTransform.GetLocation();
	const auto HeadDirection = HeadTransform.GetRotation().RotateVector(FVector::XAxisVector);

	const auto Distance = (Location - HeadLocation).Size();

	const auto Angle = FVector::DotProduct(Location - HeadLocation, HeadDirection) / Distance;

	const auto OnDirection = FMath::Abs(Angle - 1.0) < 0.01;

	// UE_LOG(LogTemp, Log, TEXT("Location = %s HeadLocation = %s HeadDirection = %s Distance = %f Angle = %f OnDirection = %d"),
	// *Location.ToString(),
	// *HeadLocation.ToString(),
	// *HeadDirection.ToString(),
	// Distance,
	// Angle,
	// OnDirection);

	if (!OnDirection)
		return true;

	if (FMath::Floor(Distance / Grid) < NearestSpawnLocation)
		return false;

	return true;
}

AActor* AMrsSnakeGameModeBase::DoSpawnItem(const TSubclassOf<AItemBase> ItemClass, FVector Location)
{
	// UE_LOG(LogTemp, Log, TEXT("Spawn item class = %s at location = %s"),
	// *ItemClass->GetName(),
	// *Location.ToString());

	const auto Item = GetMyWorld()->SpawnActor<AItemBase>(ItemClass, Location, FRotator::ZeroRotator);
	if (Item->CanGrowSnake())
		GrowItem = Item;
	SceneActors.Add(Location, Item);
	Item->OnDestroyed.AddDynamic(this, &AMrsSnakeGameModeBase::OnSceneActorDestroyed);
	return Item;
}

bool AMrsSnakeGameModeBase::SpawnItem(const FItemDescription* ItemDescription)
{
	if (!ItemDescription->IsGrowItem && FMath::FRandRange(0.0f, 1.0f) > ItemDescription->Chance)
		return false;

	const auto Spawner = NewObject<UObject>(this, ItemDescription->SpawnerClass);

	for (int Try = 0; Try < SpawnTries; Try++)
	{
		auto Locations = Cast<ISpawner>(Spawner)->Execute_GetLocations(
			Spawner, ItemDescription->SpawnerParameters, BottomLeft, TopRight, Grid);

		for (auto &It : Locations)
			It = It.GridSnap(Grid);

		if (Locations.FindByPredicate([this](const auto It) { return !CanSpawnItem(It); }))
			continue;

		for (const auto It : Locations)
			DoSpawnItem(ItemDescription->ItemClass, It);

		return true;
	}

	return false;
}

void AMrsSnakeGameModeBase::OnMrsSnakeDie(AActor* Actor)
{
	Info->Message = "Game Over... Press 'Esc' to exit";
	SetActorTickEnabled(false);
}

void AMrsSnakeGameModeBase::OnSceneActorDestroyed(AActor* Actor)
{
	const auto Item = Cast<AItemBase>(Actor);
	if (Item != nullptr)
	{
		if (Item->CanGrowSnake())
		{
			GrowItem = nullptr;
			Info->Score++;
		}
		SceneActors.Remove(Actor->GetActorLocation());
	}
}

void AMrsSnakeGameModeBase::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetMyWorld(), GetPlayer(), EQuitPreference::Quit, false);
}

void AMrsSnakeGameModeBase::StartGame()
{
	if (!CanEverStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't start game ... something wrong with environment"));
		return;
	}

	if (IsGameStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("Game already started"));
		return;
	}

	IsGameStarted = true;

	Info->Message = "";

	SetActorTickEnabled(true);
	SetActorTickInterval(0.5f);

	MrsSnake->OnDestroyed.AddDynamic(this, &AMrsSnakeGameModeBase::OnMrsSnakeDie);

	MrsSnake->ReleaseSnake();
}
