// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnitSet.h"

FUnitSet FUnitSet::Empty;


FUnitSet::FUnitSet(UUnitDescription* UnitType, int32 Amount)
{
	Add(UnitType, Amount);
}

FUnitSet::FUnitSet(UUnitDescription* UnitType, int32 Amount, TArray<AUnit*> Actors)
{
	Add(UnitType, Amount, Actors);
}

void FUnitSet::Add(const FUnitSet& OtherSet)
{
	for (auto& Entry : OtherSet.Units)
	{
		Add(Entry.Key, Entry.Value.Count, Entry.Value.ActorList);
	}
}

void FUnitSet::Add(UUnitDescription* UnitType, int32 Amount)
{
	FUnitEntry& ContainedUnits = Units.FindOrAdd(UnitType);
	ContainedUnits.Count += Amount;
}

void FUnitSet::Add(UUnitDescription* UnitType, int32 Amount, TArray<AUnit*> Actors)
{
	FUnitEntry& ContainedUnits = Units.FindOrAdd(UnitType);
	ContainedUnits.Count += Amount;
	ContainedUnits.ActorList.Append(Actors);
}

bool FUnitSet::CanRemove(const FUnitSet& UnitSet) const
{
	for (auto& Entry : UnitSet.Units)
	{
		if(!CanRemove(Entry.Key, Entry.Value.Count))
			return false;
	}
	return true;
}

bool FUnitSet::CanRemove(UUnitDescription* UnitType, int32 Amount) const
{
	ensure(UnitType);
	ensure(Amount > 0);
	const FUnitEntry* Entry = Units.Find(UnitType);
	return Entry && Entry->Count >= Amount;
}

bool FUnitSet::Remove(const FUnitSet& UnitSet, FUnitSet* OutRemovedUnits/* = nullptr*/)
{
	if(!CanRemove(UnitSet))
		return false;

	const auto UnitSetCopy = UnitSet;
	for (auto& Entry : UnitSetCopy.Units)
	{
		Remove(Entry.Key, Entry.Value, OutRemovedUnits);
	}

	return true;
}

bool FUnitSet::Remove(UUnitDescription* UnitType, const FUnitEntry& EntryToRemove, FUnitSet* OutRemovedUnits/* = nullptr*/)
{
	if(!CanRemove(UnitType, EntryToRemove.Count))
		return false;

	TArray<AUnit*> RemovedActors;

	FUnitEntry* ContainedEntry = Units.Find(UnitType);
	ContainedEntry->Count -= EntryToRemove.Count;

	if (ContainedEntry->ActorList.Num() > 0)
	{
		int Taken = 0;
		TArray<AUnit*> RequestedActors = EntryToRemove.ActorList;
		while(Taken++ < EntryToRemove.Count)
		{ 
			if (RequestedActors.Num())
			{
				auto RequestedActorPtr = RequestedActors.Pop();
				ContainedEntry->ActorList.Remove(RequestedActorPtr);
				RemovedActors.Add(RequestedActorPtr);
			}
			else
			{
				auto AnyActorPtr = ContainedEntry->ActorList.Pop();
				RemovedActors.Add(AnyActorPtr);
			}
		}
	}

	if (OutRemovedUnits)
	{
		FUnitEntry& EntryInRemovedSet = OutRemovedUnits->Units.FindOrAdd(UnitType);
		EntryInRemovedSet.Count += EntryToRemove.Count;
		EntryInRemovedSet.ActorList = RemovedActors;
	}

	if(ContainedEntry->Count == 0)
		Units.Remove(UnitType);

	return true;
}

bool FUnitSet::Remove(UUnitDescription* UnitType, int32 Amount, FUnitSet* OutRemovedUnits/* = nullptr*/)
{
	FUnitEntry Entry { Amount };
	return Remove(UnitType, Entry, OutRemovedUnits);
}

int32 FUnitSet::TotalCount() const
{
	int32 Sum = 0;
	for (auto& Entry : Units)
	{
		Sum += Entry.Value.Count;
	}
	return Sum;
}

FUnitSet FUnitSet::operator+(const FUnitSet& OtherSet) const
{
	FUnitSet Sum(*this);
	Sum.Add(OtherSet);
	return Sum;
}

FUnitSet FUnitSet::operator-(const FUnitSet& OtherSet) const
{
	FUnitSet Difference(*this);
	Difference.Remove(OtherSet);
	return Difference;
}

FUnitSet& FUnitSet::operator+=(const FUnitSet& OtherSet)
{
	Add(OtherSet);
	return *this;
}

FUnitSet& FUnitSet::operator-=(const FUnitSet& OtherSet)
{
	Remove(OtherSet);
	return *this;
}
