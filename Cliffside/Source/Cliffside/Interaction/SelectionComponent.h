// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SelectionComponent.generated.h"

UCLASS()
class USelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	int32 StencilWhenSelected = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 StencilWhenHighlighted = 2;

	UFUNCTION(BlueprintCallable)
	void SetSelected(bool bNewSelected);

	UFUNCTION(BlueprintCallable)
	void SetHighlighted(bool bNewHighlighted);

	UFUNCTION(BlueprintCallable)
	void SetAffectedComponents(TArray<UMeshComponent*> NewAffectedComponents);

private:
	void RefreshStencilState();
	void ApplyCustomDepthState(bool bCustomDepthEnabled, int32 StencilValue);

	bool bCurrentlySelected = false;
	bool bCurrentlyHighlighted = false;

	UPROPERTY(Transient)
	TArray<UMeshComponent*> AffectedComponents;

};