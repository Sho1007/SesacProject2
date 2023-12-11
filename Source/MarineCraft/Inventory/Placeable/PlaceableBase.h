// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "PlaceableBase.generated.h"

/**
 * 
 */
class AFloorBase;
UCLASS()
class MARINECRAFT_API APlaceableBase : public AItemBase
{
	GENERATED_BODY()

public:
	APlaceableBase();

	virtual void BeginPlay() override;

	virtual void SetInHand() override;
	virtual void SetInInventory() override;
	virtual void SetInWorld() override;

	virtual void Place();

	virtual void Tick(float DeltaSeconds) override;

	void SetPlaced(AFloorBase* NewPlacedFloor);

protected:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	FVector PlaceOffset;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	FVector GhostMeshOffset;

	// Trace 할 때는, Place 할 Floor, Place 할 때는, Placed 된 Floor
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	AFloorBase* PlacedFloor;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bIsPlaced;

	UPROPERTY(Meta = ( AllowPrivateAccess ) )
	TArray<UMaterialInterface*> OriginalMaterials;
};