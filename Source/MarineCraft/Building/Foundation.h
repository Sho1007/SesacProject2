		// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Building/BuildingPartsBase.h"
#include "Foundation.generated.h"

/**
 * 
 */
class ARaft;
UCLASS()
class MARINECRAFT_API AFoundation : public ABuildingPartsBase
{
	GENERATED_BODY()

public:
	void FindAdjacencyFoundation();

	ARaft* GetRaft() const;
	void SetRaft( ARaft* NewRaft );

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	UPROPERTY(EditInstanceOnly)
	ARaft* Raft;

	UPROPERTY( EditDefaultsOnly)
	float LineTraceDistance;

	UPROPERTY( EditDefaultsOnly, Category = "SFX" )
	USoundBase* AttackSound;
};