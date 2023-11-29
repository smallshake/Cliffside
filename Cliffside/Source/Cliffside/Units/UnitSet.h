// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnitSet.generated.h"

class UUnitDescription;
class AUnit;

USTRUCT(BlueprintType)
struct FUnitEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AUnit*> ActorList;

};

USTRUCT(BlueprintType)
struct FUnitSet
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<UUnitDescription*, FUnitEntry> Units;

	FUnitSet() = default;
	FUnitSet(UUnitDescription* UnitType, int32 Amount);
	FUnitSet(UUnitDescription* UnitType, int32 Amount, TArray<AUnit*> Actors);

	void Add(const FUnitSet& OtherSet);
	void Add(UUnitDescription* UnitType, int32 Amount);
	void Add(UUnitDescription* UnitType, int32 Amount, TArray<AUnit*> Actors);

	bool CanRemove(const FUnitSet& UnitSet) const;
	bool CanRemove(UUnitDescription* UnitType, int32 Amount) const;
	bool Remove(const FUnitSet& UnitSetToRemove, FUnitSet* OutRemovedUnits = nullptr);
	bool Remove(UUnitDescription* UnitType, const FUnitEntry& Entry, FUnitSet* OutRemovedUnits = nullptr);
	bool Remove(UUnitDescription* UnitType, int32 Amount, FUnitSet* OutRemovedUnits = nullptr);

	int32 TotalCount() const;

	FUnitSet operator+(const FUnitSet& OtherSet) const;
	FUnitSet operator-(const FUnitSet& OtherSet) const;

	FUnitSet& operator+=(const FUnitSet& OtherSet);
	FUnitSet& operator-=(const FUnitSet& OtherSet);

	static FUnitSet Empty;
};
