// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "ToolBase.generated.h"

UCLASS()
class MARINECRAFT_API AToolBase : public AItemBase
{
	GENERATED_BODY()
public:	
	virtual void BeginPlay() override;

	virtual void Use();
	virtual void StopUse();
	virtual void Cancel();

	float GetCurrentDurability();
	float GetMaxDurability();

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bIsChargeable;
	bool bIsCharging;
	UPROPERTY(EditDefaultsOnly)
	float MaxChargeTime;
	float CurrentChargeTime;
	UPROPERTY( EditDefaultsOnly )
	float MaxDurability;
	float CurrentDurability;
};