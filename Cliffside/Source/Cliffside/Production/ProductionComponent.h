// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProductionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWorkEvent, UProductionComponent*, ProductionComponent, int32, AddedWorkTicks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProductionEvent, UProductionComponent*, ProductionComponent);

UCLASS()
class UProductionComponent : public UActorComponent
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, meta=(ClampMin=0.1f))
	float ProductionEquationExponent = 1.f;

	UPROPERTY(EditAnywhere, meta=(ClampMin=0.001f))
	float ProductionEquationMultiplier = 1.f;
	
	UFUNCTION(BlueprintCallable)
	void SetNumberOfProducingUnits(int32 NewNumberOfUnits);

	UFUNCTION(BlueprintPure)
	int32 GetNumebrOfProducingUnits() const { return NumberOfProducingUnits; }

	UFUNCTION(BlueprintCallable)
	void AddClickProgress();

	UFUNCTION(BlueprintPure)
	float GetProductionGoal() const { return ProductionGoal;}

	UFUNCTION(BlueprintPure)
	float GetProducedWork() const { return AccumulatedWork; }

	UFUNCTION(BlueprintPure)
	float GetProductionProgressAsFraction() const { return AccumulatedWork / ProductionGoal; }

	UFUNCTION(BlueprintCallable)
	void ResumeProduction();

	UFUNCTION(BlueprintCallable)
	void StopProduction(bool bResetAccumulatedWork = true);

	UPROPERTY(BlueprintAssignable)
	FWorkEvent OnWorkAdded;

	UPROPERTY(BlueprintAssignable)
	FProductionEvent OnProductionFinished;

	UProductionComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	void TickProduction(float DeltaTime);
	void AddProduction(float WorkToAdd);

	float UnitCountToProductionValue(int32 UnitCount);

	int32 NumberOfProducingUnits = 0;

	float AccumulatedWork = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ProductionGoal = 10;

	bool bProductionEnabled = true;

};