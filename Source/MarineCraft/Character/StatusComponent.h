// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARINECRAFT_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddDamage(float DamageAmount);

	bool IsDead() const;

	void AddThirsty( float ThirstyAmount );

	void AddHunger( float HungerAmount );

private:
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float MaxHP = 100.0f;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	float CurrentHP;

	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float MaxThirsty = 100.0f;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	float CurrentThirsty;

	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float MaxHunger = 100.0f;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	float CurrentHunger;

	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bIsDead;

	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess ) )
	float Modifier = 1.0f;

public:
	DECLARE_DELEGATE_TwoParams(FDele_Single_Two_Float_Float, float, float);
	FDele_Single_Two_Float_Float OnThirstChange;
	FDele_Single_Two_Float_Float OnHungerChange;
	FDele_Single_Two_Float_Float OnHealthChange;
};