// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Engine/DataAsset.h>
#include "BuildingDescription.generated.h"

struct FResourceAmount;
class ABuilding;

UCLASS(BlueprintType)
class UBuildingDescription : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<ABuilding> BuildingActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FResourceAmount> CostToBuild;
};