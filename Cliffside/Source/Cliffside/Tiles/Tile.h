// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include <Interaction/Selectable.h>
#include "Tile.generated.h"

enum class EUnitTeam : uint8;
class ABuilding;
class AUnit;
class UUnitContainerComponent;
class USelectionComponent;
class UUnitDescription;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTileEvent);

USTRUCT(BlueprintType)
struct FUnitActorList
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AUnit*> UnitActors;
};

UCLASS()
class ATile : public AActor, public ISelectable
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHomeTile = false;

	UPROPERTY(EditDefaultsOnly)
	float RandomLocationRadius = 1000.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* UnitDetectionVolume;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABuilding* PlacedBuilding;

	UPROPERTY(BlueprintReadWrite)
	TArray<AUnit*> UnitsOnTile;

	UFUNCTION(BlueprintCallable)
	void RegisterUnitActor(AUnit* UnitActor);

	UFUNCTION(BlueprintCallable)
	void UnregisterUnitActor(AUnit* UnitActor);

	UFUNCTION(BlueprintPure)
	ABuilding* GetBuildingActor() const { return PlacedBuilding; }

	UFUNCTION(BlueprintPure)
	bool HasBuilding() const;

	UFUNCTION(BlueprintCallable)
	void RegisterBuildingActor(ABuilding* BuildingActor);

	UFUNCTION(BlueprintCallable)
	void UnregisterBuildingActor();

	UFUNCTION(BlueprintCallable)
	AUnit* AddNewUnits(UUnitDescription* UnitType, int32 UnitCount, FVector SpawnLocation, bool bForceCreateNewActor = false);

	UFUNCTION(BlueprintCallable)
	TArray<AUnit*> GetPlayerUnitActors() const;

	UFUNCTION(BlueprintCallable)
	TArray<AUnit*> GetEnemyUnitActors() const;
	
	UFUNCTION(BlueprintCallable)
	TArray<AUnit*> GetOpposingUnitActors(const EUnitTeam QuerierTeam) const;
	
	UFUNCTION(BlueprintCallable)
	TArray<AUnit*> GetAttackableOpposingUnitActors(const EUnitTeam QuerierTeam) const;

	UFUNCTION(BlueprintCallable)
	FVector SelectRandomNavMeshLocation();

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	FTransform GetBuildingSpawnTransform() const;

	UPROPERTY(BlueprintAssignable)
	FTileEvent OnBuildingPlaced;

	UPROPERTY(BlueprintAssignable)
	FTileEvent OnBuildingDestroyed;

	UFUNCTION()
	void UnitDestroyed(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	ATile();
	virtual void BeginPlay() override;

	virtual bool CanBeSelected_Implementation() const override { return true; }
	virtual USelectionComponent* GetSelectionComponent_Implementation() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<UUnitDescription*, FUnitActorList> PlayerUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<UUnitDescription*, FUnitActorList> EnemyUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USelectionComponent* SelectionComponent;

	AUnit* SpawnNewUnitActor(UUnitDescription* UnitType, const FVector& SpawnLocation);

	void FindNeighbours();
	void BuildFlowField();

	UPROPERTY(VisibleAnywhere)
	TArray<ATile*> Neighbours;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATile* TileTowardsHomeTile = nullptr;

	UPROPERTY(VisibleAnywhere)
	int32 DistanceToHomeTile = INT32_MAX;

};