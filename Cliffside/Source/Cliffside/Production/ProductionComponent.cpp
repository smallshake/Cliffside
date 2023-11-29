// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProductionComponent.h"

UProductionComponent::UProductionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProductionComponent::SetNumberOfProducingUnits(int32 NewNumberOfUnits)
{
	NumberOfProducingUnits = NewNumberOfUnits;
	if(NumberOfProducingUnits == 0)
		AccumulatedWork = 0.f;
}

void UProductionComponent::AddClickProgress()
{
	AddProduction(1.f);
}

void UProductionComponent::ResumeProduction()
{
	bProductionEnabled = true;
}

void UProductionComponent::StopProduction(bool bResetAccumulatedWork)
{
	bProductionEnabled = false;
	if(bResetAccumulatedWork)
		AccumulatedWork = 0.f;
}

void UProductionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if(bProductionEnabled)
		TickProduction(DeltaTime);
}

void UProductionComponent::TickProduction(float DeltaTime)
{
	float GeneratedWork = UnitCountToProductionValue(NumberOfProducingUnits) * DeltaTime;
	AddProduction(GeneratedWork);
}

void UProductionComponent::AddProduction(float WorkToAdd)
{
	const int32 PreviousWorkTicks = FMath::FloorToInt32(AccumulatedWork);

	AccumulatedWork += WorkToAdd;

	const int32 NewWorkTicks = FMath::FloorToInt32(AccumulatedWork);
	if(NewWorkTicks > PreviousWorkTicks)
		OnWorkAdded.Broadcast(this, NewWorkTicks - PreviousWorkTicks);

	if (ensure(ProductionGoal > 0.f))
	{
		while (AccumulatedWork >= ProductionGoal)
		{
			OnProductionFinished.Broadcast(this);
			AccumulatedWork -= ProductionGoal;
		}
	}
}

float UProductionComponent::UnitCountToProductionValue(int32 UnitCount)
{
	return FMath::Pow(UnitCount, ProductionEquationExponent) * ProductionEquationMultiplier;
}
