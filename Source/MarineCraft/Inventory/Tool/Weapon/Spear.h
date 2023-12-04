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

	virtual void CheckAttackHit() override;
	virtual void EndAttack() override;

protected :
	virtual void SetInHand() override;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float AttackRadius;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UAnimMontage* MiddleAttackMontage;

	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = (AllowPrivateAccess) )
	USoundBase* AttackSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = (AllowPrivateAccess) )
	USoundBase* StabSound;
};