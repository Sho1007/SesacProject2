// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon/WeaponBase.h"
#include "Spear.generated.h"

/**
 * 
 */
UCLASS()
class MARINECRAFT_API ASpear : public AWeaponBase
{
	GENERATED_BODY()

public:
	virtual void Use() override;

protected :
	virtual void SetInHand() override;
};