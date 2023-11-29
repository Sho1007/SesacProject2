// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ToolBase.h"
#include "WeaponBase.generated.h"

UCLASS()
class MARINECRAFT_API AWeaponBase : public AToolBase
{
	GENERATED_BODY()

public:
	virtual void CheckAttackHit();
	virtual void EndAttack();
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeaponDamage;
};