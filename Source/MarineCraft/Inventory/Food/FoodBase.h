// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Tool/ToolBase.h"
#include "FoodBase.generated.h"

/**
 * 
 */
UCLASS()
class MARINECRAFT_API AFoodBase : public AToolBase
{
	GENERATED_BODY()
public:
	virtual void Use() override;

	float GetCookTime() const;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float ThirstAmount;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float HungerAmount;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float CookTime;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	bool bIsRaw;
}; 