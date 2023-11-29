// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CliffsideWorldSettings.generated.h"

UCLASS()
class CLIFFSIDE_API ACliffsideWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D TileSize = FVector2D(1000.f, 1000.f);
	
};
