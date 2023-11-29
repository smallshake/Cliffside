// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Selectable.generated.h"

class USelectionComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class USelectable : public UInterface
{
	GENERATED_BODY()
};

class ISelectable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanBeSelected() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSelected();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDeselected();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USelectionComponent* GetSelectionComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<UUserWidget*> BuildActionList(APlayerController* DisplayingPlayer);

};