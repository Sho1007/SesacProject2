		// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Building/BuildingPartsBase.h"
#include "Foundation.generated.h"

/**
 * 
 */

UENUM()
enum class EDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right,
	Size
};

class ARaft;
UCLASS()
class MARINECRAFT_API AFoundation : public ABuildingPartsBase
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
	void DoDestroy();
	

	void FindAdjacencyFoundation();
	int32 GetAdjacencyFoundationCount() const;

	ARaft* GetRaft() const;
	void SetRaft( ARaft* NewRaft );

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	void SetAdjacencyFoundation( EDirection Direction, AFoundation* Foundation );

	void GetAdjacencyFoundation( OUT TArray<AFoundation*>& OutArray) const;

	void DestroyOnSeperated();

	bool CheckIsCorner() const;

protected:
	UPROPERTY(EditInstanceOnly)
	ARaft* Raft;

	UPROPERTY(VisibleInstanceOnly)
	AFoundation* LeftFoundation;
	UPROPERTY(VisibleInstanceOnly)
	AFoundation* RightFoundation;
	UPROPERTY(VisibleInstanceOnly)
	AFoundation* ForwardFoundation;
	UPROPERTY(VisibleInstanceOnly)
	AFoundation* BackwardFoundation;

	UPROPERTY( VisibleInstanceOnly )
	bool bIsCorner;

	UPROPERTY( EditDefaultsOnly)
	float LineTraceDistance;

	UPROPERTY( EditDefaultsOnly, Category = "SFX" )
	USoundBase* AttackSound;
};