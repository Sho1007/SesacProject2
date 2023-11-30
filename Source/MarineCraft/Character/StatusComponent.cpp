// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	CurrentHP = MaxHP;
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatusComponent::AddDamage(float DamageAmount)
{
	if ( bIsDead ) return;

	UE_LOG( LogTemp , Warning , TEXT( "UStatusComponent::AddDamage) DamageAmount : %f" ), DamageAmount);

	CurrentHP -= DamageAmount;

	if (CurrentHP <= 0.0f )
	{
		// Death
		CurrentHP = 0.0f;
		bIsDead = true;
		// Todo : Call Death Function Of Owner Character
	}
}

bool UStatusComponent::IsDead() const
{
	return bIsDead;
}