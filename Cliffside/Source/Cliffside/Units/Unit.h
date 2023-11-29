// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/Damageable.h"
#include "Interaction/Selectable.h"
#include "Unit.generated.h"

class ATile;
class UUnitDescription;
class UUnitContainerComponent;

UENUM(BlueprintType)
enum class EUnitTeam : uint8
{
	PlayerTeam,
	EnemyTeam
};

UCLASS()
class AUnit : public ACharacter, public ISelectable, public IDamageable
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	EUnitTeam Team;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDescription(UUnitDescription* UnitDescription);
	virtual void SetDescription_Implementation(UUnitDescription* UnitDescription);

	UFUNCTION(BlueprintPure)
	UUnitDescription* GetUnitDescription() const { return Description; }

	UFUNCTION(BlueprintPure)
	int32 GetUnitCount() const;

	UFUNCTION(BlueprintPure)
	int32 GetSelectedUnitCount() const;

	UFUNCTION(BlueprintCallable)
	void AddUnitCount(int32 CountToAdd);

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanAcceptMoreUnits() const;

	UFUNCTION(BlueprintCallable)
	void DealDamage(UObject* DamageableObject);

	UFUNCTION(BlueprintCallable, meta=(WorldContext=WorldContextObject))
	static AUnit* SpawnUnitActor(UObject* WorldContextObject, UUnitDescription* UnitType, int32 UnitStrength, EUnitTeam UnitTeam, FTransform SpawnTransform);

	AUnit();
	virtual void BeginPlay() override;

	void SetOccupiedTile(ATile* NewOccupiedTile);
	ATile* GetOccupiedTile() const { return OccupiedTile; }

	void SetTeam(EUnitTeam InTeam) { Team = InTeam; }

	virtual bool CanBeDamaged_Implementation() const override { return true; }
	virtual void TakeDamage_Implementation(float DamageAmount, UObject* Source) override;

	virtual bool CanBeSelected_Implementation() const override;
	virtual USelectionComponent* GetSelectionComponent_Implementation() override { return SelectionComponent; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USelectionComponent* SelectionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUnitContainerComponent* UnitContainerComponent;

	UPROPERTY(BlueprintReadWrite)
	ATile* OccupiedTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn))
	UUnitDescription* Description;

	float MarkedDamage = 0.f;

private:

	UFUNCTION()
	void UnitCountChanged(UUnitContainerComponent* InUnitContainerComponent);
};