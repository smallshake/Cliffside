// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Character.h>
#include <Engine/DataAsset.h>
#include "UnitDescription.generated.h"

class ATile;
class AUnit;

UCLASS(BlueprintType)
class UUnitDescription : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Power = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxHealth = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AUnit> UnitActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimMontage*> MontageBank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UAnimSequence*> AnimSequenceBank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UBlendSpace*> BlendspaceBank;
};