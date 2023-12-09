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
	void PutWater(bool NewPurified);
	void PourWater();

	bool HasWater() const;
	bool Purified() const;

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bHasWater;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsPurified;
};