// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Inventory/FloatsamBase.h"

#include "Barrel.generated.h"

/**
 * 
 */
UCLASS()
class MARINECRAFT_API ABarrel : public AFloatsamBase
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacter* InteractCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TMap<FName, float> LootItemArray;
};