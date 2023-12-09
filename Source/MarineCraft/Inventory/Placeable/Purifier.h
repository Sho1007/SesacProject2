// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Placeable/PlaceableBase.h"
#include "Purifier.generated.h"

/**
 * 
 */
class APlayerController;
UCLASS()
class MARINECRAFT_API APurifier : public APlaceableBase
{
	GENERATED_BODY()
	
public:
	virtual void Interact(ACharacter* InteractCharacter) override;
	virtual FText GetInteractActorName( APlayerController* InteractPlayerController ) override;

	void Boil();

	virtual void Tick(float DeltaSeconds) override;

private:
	/*UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	TSubclassOf<A>*/

	// State
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bIsBoiling;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bIsPurified;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bHasCupOfWater;

	// Fuel
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	int32 FuelCount;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	float CurrentFuelTime;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float MaxFuelTime;

	// Purify
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	float CurrentPurifyingTime;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float MaxPurifyingTime;
};