// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Tool/ToolBase.h"
#include "BuildingHammer.generated.h"

/**
 * 
 */

struct FBuildingPartsData;
UCLASS()
class MARINECRAFT_API ABuildingHammer : public AToolBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetInHand() override;
	virtual void SetInInventory() override;
	virtual void SetInWorld() override;

	virtual void Use() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Build" , Meta = (AllowPrivateAccess))
	float TraceDistance = 1000.0f;

	UPrimitiveComponent* BuildTargetComponent;

	FBuildingPartsData* BuildingPartsData;

	UPROPERTY( EditDefaultsOnly , Category = "Animation" , Meta = ( AllowPrivateAccess ) )
	UAnimMontage* BuildMontage;
};