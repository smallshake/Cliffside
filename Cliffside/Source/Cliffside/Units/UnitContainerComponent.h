// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnitSet.h"
#include "UnitContainerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitContainerEvent, UUnitContainerComponent*, UnitContainerComponent);

UCLASS()
class UUnitContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UUnitContainerComponent();

	UFUNCTION(BlueprintCallable)
	void AddUnits(const FUnitSet& Units);

	UFUNCTION(BlueprintCallable)
	FUnitSet RemoveUnits(const FUnitSet& UnitsToRemove);

	UFUNCTION(BlueprintPure)
	FUnitSet GetContainedUnits() const { return ContainedUnits; }

	UFUNCTION(BlueprintCallable)
	TArray<UUnitDescription*> GetTypesOfUnits() const;

	UFUNCTION(BlueprintPure)
	int32 GetNumberOfUnitsOfType(UUnitDescription* UnitType) const;

	UFUNCTION(BlueprintPure)
	int32 GetNumberOfSelectedUnitsOfType(UUnitDescription* UnitType) const;
	
	UFUNCTION(BlueprintPure)
	int32 GetTotalNumberOfSelectedUnits() const;

	UFUNCTION(BlueprintPure)
	int32 GetTotalNumberOfContainedUnits() const;

	UFUNCTION(BlueprintCallable)
	void SelectUnitOfType(UUnitDescription* UnitType);

	UFUNCTION(BlueprintCallable)
	void SelectUnits(const FUnitSet& UnitSet);

	UFUNCTION(BlueprintCallable)
	void SelectAllUnits();

	UFUNCTION(BlueprintCallable)
	void DeselectAllUnits();

	UPROPERTY(BlueprintAssignable)
	FUnitContainerEvent OnContainedUnitsChanged;

	UPROPERTY(BlueprintAssignable)
	FUnitContainerEvent OnSelectedUnitsChanged;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUnitSet ContainedUnits;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUnitSet SelectedUnits;

};