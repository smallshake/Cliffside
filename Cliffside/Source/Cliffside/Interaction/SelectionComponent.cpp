// Copyright Epic Games, Inc. All Rights Reserved.

#include "SelectionComponent.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"

void USelectionComponent::SetSelected(bool bNewSelected)
{
	if (bNewSelected != bCurrentlySelected)
	{
		bCurrentlySelected = bNewSelected;
		RefreshStencilState();
	}
}

void USelectionComponent::SetHighlighted(bool bNewHighlighted)
{
	if (bNewHighlighted != bCurrentlyHighlighted)
	{
		bCurrentlyHighlighted = bNewHighlighted;
		RefreshStencilState();
	}
}

void USelectionComponent::SetAffectedComponents(TArray<UMeshComponent*> NewAffectedComponents)
{
	AffectedComponents = NewAffectedComponents;
}

void USelectionComponent::RefreshStencilState()
{
	const bool bCustomDepthEnabled = bCurrentlySelected || bCurrentlyHighlighted;
	const int32 StencilValue = (bCurrentlySelected ? StencilWhenSelected : 0)
		+ (bCurrentlyHighlighted ? StencilWhenHighlighted : 0);
	ApplyCustomDepthState(bCustomDepthEnabled, StencilValue);
}

void USelectionComponent::ApplyCustomDepthState(bool bCustomDepthEnabled, int32 StencilValue)
{
	if(AffectedComponents.Num() == 0)
	{
		const AActor* OwnerActor = GetOwner();
		TArray<UMeshComponent*> PrimitiveComponents;
		OwnerActor->GetComponents<UMeshComponent>(PrimitiveComponents);
		AffectedComponents.Append(PrimitiveComponents);
	}

	for (UMeshComponent* Component : AffectedComponents)
	{
		if (Component->IsA<UWidgetComponent>())
			continue;

		Component->SetRenderCustomDepth(bCustomDepthEnabled);
		Component->SetCustomDepthStencilValue(StencilValue);
	}
}
