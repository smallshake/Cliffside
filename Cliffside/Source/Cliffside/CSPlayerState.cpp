// Copyright Epic Games, Inc. All Rights Reserved.


#include "CSPlayerState.h"

#include "Kismet/GameplayStatics.h"

void ACSPlayerState::AddResource(UResourceDescription* ResourceType, int Amount)
{
	auto& State = Resources.FindOrAdd(ResourceType);
	State.CurrentAmount += Amount;
	OnResourceStateChanged.Broadcast(ResourceType);
}

bool ACSPlayerState::CanPayResources(const TArray<FResourceAmount>& ResourceCost) const
{
	for(const auto& CostEntry : ResourceCost)
	{
		const int32 OwnedAmount = GetResourceCurrentAmount(CostEntry.ResourceType);
		if(OwnedAmount < CostEntry.Amount)
			return false;
	}
	return true;
}

bool ACSPlayerState::PayResources(const TArray<FResourceAmount>& ResourceCost)
{
	if(CanPayResources(ResourceCost))
	{
		for (const auto& CostEntry : ResourceCost)
		{
			auto* State = Resources.Find(CostEntry.ResourceType);
			if(State)
			{
				State->CurrentAmount -= CostEntry.Amount;
			}
		}

		for (const auto& CostEntry : ResourceCost)
		{
			OnResourceStateChanged.Broadcast(CostEntry.ResourceType);
		}
		return true;
	}
	else
	{
		return false;
	}
}

int32 ACSPlayerState::GetResourceCurrentAmount(const UResourceDescription* ResourceType) const
{
	auto* State = Resources.Find(ResourceType);
	if(State)
	{
		return State->CurrentAmount;
	}
	else
	{
		return 0;
	}
}

TArray<UResourceDescription*> ACSPlayerState::GetOwnedResourceTypes() const
{
	TArray<UResourceDescription*> ResourceTypes;
	Resources.GenerateKeyArray(ResourceTypes);
	return ResourceTypes;
}

ACSPlayerState* ACSPlayerState::GetCSPlayerState(const UObject* WorldContextObject, int32 PlayerStateIndex)
{
	return Cast<ACSPlayerState>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerStateIndex));
}
