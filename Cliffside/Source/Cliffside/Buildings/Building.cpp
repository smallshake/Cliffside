// Copyright Epic Games, Inc. All Rights Reserved.

#include "Building.h"

#include "BuildingDescription.h"
#include "Production/ProductionComponent.h"
#include "Units/UnitContainerComponent.h"
#include "Interaction/SelectionComponent.h"
#include "Tiles/Tile.h"

void ABuilding::SetDescription_Implementation(UBuildingDescription* BuildingDescription)
{
	Description = BuildingDescription;
}

ABuilding* ABuilding::SpawnBuildingActor(UObject* WorldContextObject, UBuildingDescription* BuildingType,
                                         FTransform SpawnTransform)
{
	auto* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (!World)
		return nullptr;

	if (!BuildingType)
		return nullptr;

	const TSubclassOf<ABuilding> BuildingActorClass = BuildingType->BuildingActorClass.LoadSynchronous();
	FActorSpawnParameters SpawnParams;
	SpawnParams.bDeferConstruction = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto* BuildingActor = World->SpawnActor<ABuilding>(BuildingActorClass, SpawnTransform, SpawnParams);
	if (ensure(BuildingActor))
	{
		BuildingActor->SetDescription(BuildingType);
		BuildingActor->FinishSpawning(SpawnTransform, true);
	}
	return BuildingActor;
}

ABuilding::ABuilding()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	ProductionComponent = CreateDefaultSubobject<UProductionComponent>(TEXT("ProductionComponent"));
	UnitContainerComponent = CreateDefaultSubobject<UUnitContainerComponent>(TEXT("UnitContainerComponent"));
	SelectionComponent = CreateDefaultSubobject<USelectionComponent>(TEXT("SelectionComponent"));
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	RemainingHealth = MaxHealth;

	UnitContainerComponent->OnContainedUnitsChanged.AddDynamic(this, &ABuilding::OnContainedUnitsChanged);
}

void ABuilding::TakeDamage_Implementation(float DamageAmount, UObject* Source)
{
	RemainingHealth -= DamageAmount;
	if(RemainingHealth <= 0.f)
	{
		OccupiedTile->UnregisterBuildingActor();
		Destroy();
	}
}

USelectionComponent* ABuilding::GetSelectionComponent_Implementation()
{
	return SelectionComponent;
}

void ABuilding::OnContainedUnitsChanged(UUnitContainerComponent* UnitContainer)
{
	int32 UnitsInBuilding = UnitContainerComponent->GetTotalNumberOfContainedUnits();
	ProductionComponent->SetNumberOfProducingUnits(UnitsInBuilding);
}

