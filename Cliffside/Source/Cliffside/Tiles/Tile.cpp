// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tile.h"
#include "Units/Unit.h"
#include "NavigationSystem.h"
#include "Interaction/SelectionComponent.h"
#include <Units/UnitDescription.h>
#include "Buildings/Building.h"

#include "CliffsideWorldSettings.h"
#include "EngineUtils.h"

ATile::ATile()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootScene);

	UnitDetectionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("UnitDetectionComponent"));
	UnitDetectionVolume->SetupAttachment(RootScene);

	SelectionComponent = CreateDefaultSubobject<USelectionComponent>(TEXT("SelectionComponent"));
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	if(bHomeTile)
	{
		DistanceToHomeTile = 0;
		BuildFlowField();
	}
}

void ATile::RegisterUnitActor(AUnit* UnitActor)
{
	if(!ensure(IsValid(UnitActor)))
		return;

	if (auto* OldTile = UnitActor->GetOccupiedTile())
	{
		OldTile->UnregisterUnitActor(UnitActor);
	}
	UnitActor->SetOccupiedTile(this);

	if(UnitActor->Team == EUnitTeam::PlayerTeam)
	{
		auto& Entry = PlayerUnits.FindOrAdd(UnitActor->GetUnitDescription());
		Entry.UnitActors.Add(UnitActor);
	}
	else if(UnitActor->Team == EUnitTeam::EnemyTeam)
	{
		auto& Entry = EnemyUnits.FindOrAdd(UnitActor->GetUnitDescription());
		Entry.UnitActors.Add(UnitActor);
	}

	UnitActor->OnEndPlay.AddDynamic(this, &ThisClass::UnitDestroyed);
}

void ATile::UnregisterUnitActor(AUnit* UnitActor)
{
	UnitActor->OnEndPlay.RemoveDynamic(this, &ThisClass::UnitDestroyed);

	UnitActor->SetOccupiedTile(nullptr);

	auto& TeamUnitList = UnitActor->Team == EUnitTeam::PlayerTeam ? PlayerUnits : EnemyUnits;
	auto* Entry = TeamUnitList.Find(UnitActor->GetUnitDescription());
	if(ensure(Entry) && ensure(Entry->UnitActors.Contains(UnitActor)))
	{
		Entry->UnitActors.Remove(UnitActor);
	}
}

bool ATile::HasBuilding() const
{
	return IsValid(PlacedBuilding);
}

void ATile::RegisterBuildingActor(ABuilding* BuildingActor)
{
	ensure(!PlacedBuilding);
	PlacedBuilding = BuildingActor;
	PlacedBuilding->OccupiedTile = this;
	OnBuildingPlaced.Broadcast();
}

void ATile::UnregisterBuildingActor()
{
	PlacedBuilding = nullptr;
	OnBuildingDestroyed.Broadcast();
}

AUnit* ATile::SpawnNewUnitActor(UUnitDescription* UnitType, const FVector& SpawnLocation)
{
	const TSubclassOf<AUnit> UnitActorClass = UnitType->UnitActorClass.LoadSynchronous();
	const FTransform SpawnTransform(SpawnLocation != FVector::ZeroVector ? SpawnLocation : SelectRandomNavMeshLocation());
	FActorSpawnParameters SpawnParams;
	SpawnParams.bDeferConstruction = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto* UnitActor = GetWorld()->SpawnActor<AUnit>(UnitActorClass, SpawnTransform, SpawnParams);
	if(ensure(UnitActor))
	{
		UnitActor->SetDescription(UnitType);
		UnitActor->FinishSpawning(SpawnTransform, true);

		RegisterUnitActor(UnitActor);
	}
	return UnitActor;
}

void ATile::FindNeighbours()
{
	if(Neighbours.Num() > 0)
		return;

	const auto* WorldSettings = Cast<ACliffsideWorldSettings>(GetWorld()->GetWorldSettings());
	const float GridSize = WorldSettings->TileSize.X;

	for (TActorIterator<ATile> It(GetWorld()); It; ++It)
	{
		ATile* Tile = *It;
		if(Tile == this)
			continue;
			
		const float Distance = (Tile->GetActorLocation() - this->GetActorLocation()).Length();
		if(FMath::IsNearlyEqual(GridSize, Distance, GridSize / 10))
			Neighbours.Add(Tile);
	}
}

void ATile::BuildFlowField()
{
	FindNeighbours();

	for(auto* NeighbourTile : Neighbours)
	{
		if(NeighbourTile->DistanceToHomeTile > DistanceToHomeTile + 1)
		{
			NeighbourTile->DistanceToHomeTile = DistanceToHomeTile + 1;
			NeighbourTile->TileTowardsHomeTile = this;
			NeighbourTile->BuildFlowField();
		}
	}
}

AUnit* ATile::AddNewUnits(UUnitDescription* UnitType, int32 UnitCount, FVector SpawnLocation, bool bForceCreateNewActor /*= false*/)
{
	AUnit* EligibleUnitActor = nullptr;

	if (!bForceCreateNewActor && PlayerUnits.Contains(UnitType))
	{
		for(auto* ExistingActor : PlayerUnits[UnitType].UnitActors)
		{
			if(IsValid(ExistingActor) && ExistingActor->CanAcceptMoreUnits())
			{
				EligibleUnitActor = ExistingActor;
				break;
			}
		}
	}

	if(!EligibleUnitActor)
	{
	//There is no actor for units of this type, spawn one
		EligibleUnitActor = SpawnNewUnitActor(UnitType, SpawnLocation);
	}
	
	if(ensure(EligibleUnitActor))
	{
		EligibleUnitActor->AddUnitCount(UnitCount);
		return EligibleUnitActor;
	}

	ensureMsgf(false, TEXT("Shouldn't ever get here, actor spawning must have failed."));
	return nullptr;
}

TArray<AUnit*> ATile::GetPlayerUnitActors() const
{
	TArray<AUnit*> PlayerUnitActors;
	for(auto& Entry : PlayerUnits)
	{
		PlayerUnitActors.Append(Entry.Value.UnitActors);
	}
	return PlayerUnitActors;
}

TArray<AUnit*> ATile::GetEnemyUnitActors() const
{
	TArray<AUnit*> PlayerUnitActors;
	for (auto& Entry : EnemyUnits)
	{
		PlayerUnitActors.Append(Entry.Value.UnitActors);
	}
	return PlayerUnitActors;
}

TArray<AUnit*> ATile::GetOpposingUnitActors(const EUnitTeam QuerierTeam) const
{
	if(QuerierTeam == EUnitTeam::PlayerTeam)
	{
		return GetEnemyUnitActors();
	}
	else if(QuerierTeam == EUnitTeam::EnemyTeam)
	{
		return GetPlayerUnitActors();
	}

	ensure(false);
	return TArray<AUnit*>();
}

TArray<AUnit*> ATile::GetAttackableOpposingUnitActors(const EUnitTeam QuerierTeam) const
{
	TArray<AUnit*> AttackableUnits = GetOpposingUnitActors(QuerierTeam);
	AttackableUnits = AttackableUnits.FilterByPredicate(
		[](const AUnit* Unit)
		{
			return IDamageable::Execute_CanBeDamaged(Unit);
		});
	return AttackableUnits;
}

FVector ATile::SelectRandomNavMeshLocation()
{
	FVector OutSelectedLocation = GetActorLocation();
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, GetActorLocation(), OutSelectedLocation, RandomLocationRadius);
	return OutSelectedLocation;
}

void ATile::UnitDestroyed(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	if(!IsActorBeingDestroyed())
	{
		UnregisterUnitActor(Cast<AUnit>(Actor));
	}
}

USelectionComponent* ATile::GetSelectionComponent_Implementation()
{
	return SelectionComponent;
}
