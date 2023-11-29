// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Selectable.h"
#include "Interaction/Damageable.h"
#include "Building.generated.h"

class UBuildingDescription;
class ATile;
class UProductionComponent;
class UUnitContainerComponent;
class USelectionComponent;

UCLASS()
class ABuilding : public AActor, public ISelectable, public IDamageable
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATile* OccupiedTile = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxHealth = 100;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDescription(UBuildingDescription* BuildingDescription);
	virtual void SetDescription_Implementation(UBuildingDescription* BuildingDescription);

	UFUNCTION(BlueprintPure)
	UBuildingDescription* GetBuildingDescription() const { return Description; }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FVector GetUnitSpawnLocation() const;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = WorldContextObject))
	static ABuilding* SpawnBuildingActor(UObject* WorldContextObject, UBuildingDescription* BuildingType, FTransform SpawnTransform);

	ABuilding();

	virtual void BeginPlay() override;

	virtual bool CanBeDamaged_Implementation() const override { return true; }
	virtual void TakeDamage_Implementation(float DamageAmount, UObject* Source) override;

	virtual bool CanBeSelected_Implementation() const override { return true; }
	virtual USelectionComponent* GetSelectionComponent_Implementation() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProductionComponent* ProductionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUnitContainerComponent* UnitContainerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USelectionComponent* SelectionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn))
	UBuildingDescription* Description;

	float RemainingHealth;

private:

	UFUNCTION()
	void OnContainedUnitsChanged(UUnitContainerComponent* UnitContainer);
};