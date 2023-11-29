// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Production/ResourceDescription.h"
#include "CSPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResourceEvent, UResourceDescription*, ResourceType);

USTRUCT(BlueprintType)
struct FResourceState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentAmount = 0;
};

UCLASS()
class CLIFFSIDE_API ACSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddResource(UResourceDescription* ResourceType, int Amount);


	UFUNCTION(BlueprintCallable)
	bool CanPayResources(const TArray<FResourceAmount>& ResourceCost) const;

	UFUNCTION(BlueprintCallable)
	bool PayResources(const TArray<FResourceAmount>& ResourceCost);

	UFUNCTION(BlueprintPure)
	int32 GetResourceCurrentAmount(const UResourceDescription* ResourceType) const;

	UFUNCTION(BlueprintPure)
	TArray<UResourceDescription*> GetOwnedResourceTypes() const;

	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static ACSPlayerState* GetCSPlayerState(const UObject* WorldContextObject, int32 PlayerStateIndex);

	UPROPERTY(BlueprintAssignable)
	FResourceEvent OnResourceStateChanged;



protected:

	UPROPERTY()
	TMap<UResourceDescription*, FResourceState> Resources;
};
