// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Engine/DataAsset.h>
#include "ResourceDescription.generated.h"


UCLASS(BlueprintType)
class UResourceDescription : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FResourceAmount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UResourceDescription* ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};
