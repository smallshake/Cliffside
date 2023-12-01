// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unit.h"
#include "UnitContainerComponent.h"
#include "UnitDescription.h"
#include "Tiles/Tile.h"
#include "Interaction/SelectionComponent.h"

void AUnit::SetDescription_Implementation(UUnitDescription* UnitDescription)
{
	Description = UnitDescription;
}

int32 AUnit::GetUnitCount() const
{
	return UnitContainerComponent->GetNumberOfUnitsOfType(Description);
}

int32 AUnit::GetSelectedUnitCount() const
{
	return UnitContainerComponent->GetNumberOfSelectedUnitsOfType(Description);
}

void AUnit::AddUnitCount(int32 CountToAdd)
{
	const FUnitSet NewUnits{GetUnitDescription(), CountToAdd};
	UnitContainerComponent->AddUnits(NewUnits);
}

bool AUnit::CanAcceptMoreUnits_Implementation() const
{
	return true;
}

void AUnit::DealDamage(UObject* DamageableObject)
{
	if(IsValid(DamageableObject) && DamageableObject->Implements<UDamageable>() && IDamageable::Execute_CanBeDamaged(DamageableObject))
	{
		const int32 BasePower = Description->Power;
		const int32 UnitCount = GetUnitCount();
		float DamageToDeal = BasePower * UnitCount;

		IDamageable::Execute_TakeDamage(DamageableObject, DamageToDeal, this);
	}
}

AUnit* AUnit::SpawnUnitActor(UObject* WorldContextObject, UUnitDescription* UnitType, int32 UnitStrength, EUnitTeam UnitTeam,
                             FTransform SpawnTransform)
{
	auto* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if(!World)
		return nullptr;

	if(!UnitType)
		return nullptr;

	const TSubclassOf<AUnit> UnitActorClass = UnitType->UnitActorClass.LoadSynchronous();
	FActorSpawnParameters SpawnParams;
	SpawnParams.bDeferConstruction = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto* UnitActor = World->SpawnActor<AUnit>(UnitActorClass, SpawnTransform, SpawnParams);
	if (ensure(UnitActor))
	{
		UnitActor->SetDescription(UnitType);
		UnitActor->AddUnitCount(UnitStrength);
		UnitActor->SetTeam(UnitTeam);
		UnitActor->FinishSpawning(SpawnTransform, true);
	}
	return UnitActor;
}

FUnitSet AUnit::ConvertToOtherType(const FUnitSet& UnitSet, UUnitDescription* NewType)
{
	FUnitSet WorkerSet;
	WorkerSet.Add(NewType, UnitSet.TotalCount());
	return WorkerSet;

}

AUnit::AUnit()
{
	SelectionComponent = CreateDefaultSubobject<USelectionComponent>(TEXT("SelectionComponent"));
	UnitContainerComponent = CreateDefaultSubobject<UUnitContainerComponent>(TEXT("UnitContainerComponent"));
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

	UnitContainerComponent->OnContainedUnitsChanged.AddDynamic(this, &AUnit::UnitCountChanged);
}

void AUnit::SetOccupiedTile(ATile* NewOccupiedTile)
{
	OccupiedTile = NewOccupiedTile;
}

void AUnit::TakeDamage_Implementation(float DamageAmount, UObject* Source)
{
	const int32 SingleUnitHealth = Description->MaxHealth;
	const float DamageToResolve = MarkedDamage + DamageAmount;

	const int32 UnitsKilled = FMath::FloorToInt32(DamageToResolve / SingleUnitHealth);
	const float LeftoverDamage = DamageToResolve - UnitsKilled * SingleUnitHealth;

	const int32 UnitCount = GetUnitCount();

	if(UnitsKilled >= UnitCount)
	{
		Destroy();
	}
	else
	{
		if(UnitsKilled > 0)
		{
			const FUnitSet UnitsToRemove { Description, UnitsKilled };
			UnitContainerComponent->RemoveUnits(UnitsToRemove);
		}
		MarkedDamage = LeftoverDamage;
	}
}

bool AUnit::CanBeSelected_Implementation() const
{
	return Team == EUnitTeam::PlayerTeam;
}

void AUnit::UnitCountChanged(UUnitContainerComponent* InUnitContainerComponent)
{
	const int32 NewCount = UnitContainerComponent->GetTotalNumberOfContainedUnits();
	if(NewCount == 0)
	{
		OccupiedTile->UnregisterUnitActor(this);
		Destroy();
	}
}
