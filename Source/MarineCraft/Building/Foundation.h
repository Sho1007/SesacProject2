// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Building/BuildingPartsBase.h"
#include "Foundation.generated.h"

/**
 * 
 */
UCLASS()
class MARINECRAFT_API AFoundation : public ABuildingPartsBase
{
	GENERATED_BODY()

public:
	void FindAdjacencyFoundation();
	
private:
	TArray<AFoundation*> AdjacencyFoundationArray;

	UPROPERTY( EditDefaultsOnly , Meta = (AllowPrivateAccess))
	float LineTraceDistance;
};