// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Tool/ToolBase.h"
#include "Cup.generated.h"

/**
 * 
 */
UCLASS()
class MARINECRAFT_API ACup : public AToolBase
{
	GENERATED_BODY()

public:
	ACup();

	void PutWater(bool NewPurified);
	void PourWater();

	virtual void Use() override;
	virtual void Cancel() override;

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bHasWater;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsPurified;

	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	UStaticMeshComponent* WaterMeshComponent;

	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	UTexture2D* EmptyCupImage;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	UTexture2D* CupOfWaterImage;
};