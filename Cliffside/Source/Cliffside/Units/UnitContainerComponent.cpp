// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnitContainerComponent.h"

// UProductionComponent::UProductionComponent()
// {
// }
// 
// void UProductionComponent::SetNumberOfProducingUnits(int32 NewNumberOfUnits)
// {
// 	NumberOfProducingUnits = NewNumberOfUnits;
// }
// 
// void UProductionComponent::AddClickProgress()
// {
// 	AddProduction(1.f);
// }
// 
// void UProductionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	TickProduction(DeltaTime);
// }
// 
// void UProductionComponent::TickProduction(float DeltaTime)
// {
// 	float GeneratedWork = NumberOfProducingUnits * DeltaTime;
// 	AddProduction(GeneratedWork);
// }
// 
// void UProductionComponent::AddProduction(float WorkToAdd)
// {
// 	const int32 PreviousWorkTicks = FMath::FloorToInt32(AccumulatedWork);
// 
// 	AccumulatedWork += WorkToAdd;
// 
// 	const int32 NewWorkTicks = FMath::FloorToInt32(AccumulatedWork);
// 	if(NewWorkTicks > PreviousWorkTicks)
// 		OnWorkAdded.Broadcast(this, NewWorkTicks - PreviousWorkTicks);
// 
// 	if (ensure(ProductionGoal > 0.f))
// 	{
// 		while (AccumulatedWork >= ProductionGoal)
// 		{
// 			OnProductionFinished.Broadcast(this);
// 			AccumulatedWork -= ProductionGoal;
// 		}
// 	}
// }

UUnitContainerComponent::UUnitContainerComponent()
{

}

void UUnitContainerComponent::AddUnits(const FUnitSet& Units)
{
	ContainedUnits.Add(Units);
	OnContainedUnitsChanged.Broadcast(this);
}

FUnitSet UUnitContainerComponent::RemoveUnits(const FUnitSet& UnitsToRemove)
{
	FUnitSet RemovedUnits;
	ContainedUnits.Remove(UnitsToRemove, &RemovedUnits);
	OnContainedUnitsChanged.Broadcast(this);
	return RemovedUnits;
}

TArray<UUnitDescription*> UUnitContainerComponent::GetTypesOfUnits() const
{
	TArray<UUnitDescription*> UnitTypes;
	ContainedUnits.Units.GenerateKeyArray(UnitTypes);
	return UnitTypes;
}

int32 UUnitContainerComponent::GetNumberOfUnitsOfType(UUnitDescription* UnitType) const
{
	auto* Entry = ContainedUnits.Units.Find(UnitType);
	if(Entry)
		return Entry->Count;
	else
		return 0;
}

int32 UUnitContainerComponent::GetNumberOfSelectedUnitsOfType(UUnitDescription* UnitType) const
{
	auto* Entry = SelectedUnits.Units.Find(UnitType);
	if (Entry)
		return Entry->Count;
	else
		return 0;
}

int32 UUnitContainerComponent::GetTotalNumberOfSelectedUnits() const
{
	return SelectedUnits.TotalCount();
}

int32 UUnitContainerComponent::GetTotalNumberOfContainedUnits() const
{
	return ContainedUnits.TotalCount();
}

void UUnitContainerComponent::SelectUnitOfType(UUnitDescription* UnitType)
{
	auto NotSelectedUnits = ContainedUnits - SelectedUnits;
	FUnitSet NewlySelectedUnits;
	FUnitSet UnitsToSelect;
	NotSelectedUnits.Remove(UnitType, 1, &NewlySelectedUnits);
	SelectedUnits.Add(NewlySelectedUnits);
	OnSelectedUnitsChanged.Broadcast(this);
}

void UUnitContainerComponent::SelectUnits(const FUnitSet& UnitSet)
{
	DeselectAllUnits();
	SelectedUnits = UnitSet;
	OnSelectedUnitsChanged.Broadcast(this);
}

void UUnitContainerComponent::DeselectAllUnits()
{
	SelectedUnits = FUnitSet();
	OnSelectedUnitsChanged.Broadcast(this);
}
