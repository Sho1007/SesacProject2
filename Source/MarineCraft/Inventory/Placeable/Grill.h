// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Placeable/PlaceableBase.h"
#include "Grill.generated.h"

class AFoodBase;
/**
 * 
 */
UCLASS()
class MARINECRAFT_API AGrill : public APlaceableBase
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacter* InteractCharacter) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FText GetInteractActorName(APlayerController* InteractPlayerController) override;

private:
	void Cook();

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	int32 CurrentFuelCount;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	int32 MaxFuelCount;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsCooked;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsCooking;

	// Cooking Time
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	float CurrentCookingTime;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float MaxCookingTime;

	// Fuel Time
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	float CurrentFuelTime;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float MaxFuelTime;
	
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AFoodBase* CurrentFood;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TMap<FName, FName> FoodMap;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TMap<FName, FVector> GrillOffsetMap;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TArray<FName> CookableFoodNameArray;
};