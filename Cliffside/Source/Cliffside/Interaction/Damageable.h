// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Damageable.generated.h"

class USelectionComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class IDamageable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanBeDamaged() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeDamage(float DamageAmount, UObject* Source);
};