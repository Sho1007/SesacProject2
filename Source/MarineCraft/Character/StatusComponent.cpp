// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/StatusComponent.h"

#include "CharacterBase.h"
#include "MarineCraft/PlayerController/InGamePlayerController.h"

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

	// Todo : Load된 경우 Max값으로 설정하지 말고, Load된 값으로 설정
	CurrentHP = MaxHP;
	CurrentThirsty = MaxThirsty;
	CurrentHunger = MaxHunger;
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// Hunger
	if (CurrentHunger > 0.0f)
	{
		// 시간이 지남에 따라 배고픔 감소
		AddHunger(-0.09f * DeltaTime * Modifier);
	}
	else
	{
		// 허기가 고갈되면
		AddDamage(0.75f * DeltaTime * Modifier );
	}

	// Thirsty
	if ( CurrentThirsty > 0.0f )
	{
		// 시간이 지남에 따라 배고픔 감소
		AddThirsty( -0.11f * DeltaTime * Modifier );
	}
	else
	{
		// 허기가 고갈되면
		AddDamage( 0.75f * DeltaTime * Modifier );
	}
}

void UStatusComponent::AddDamage(float DamageAmount)
{
	if ( bIsDead ) return;

	// UE_LOG( LogTemp , Warning , TEXT( "UStatusComponent::AddDamage) DamageAmount : %f" ), DamageAmount);

	Cast<ACharacter>(GetOwner())->GetController<AInGamePlayerController>()->Impact( DamageAmount );

	CurrentHP -= DamageAmount;

	if (CurrentHP <= 0.0f )
	{
		// Death
		CurrentHP = 0.0f;
		bIsDead = true;

		GetOwner<ACharacterBase>()->Die();
	}

	OnHealthChange.ExecuteIfBound(CurrentHP, MaxHP);
}

bool UStatusComponent::IsDead() const
{
	return bIsDead;
}

void UStatusComponent::AddThirsty(float ThirstyAmount)
{
	CurrentThirsty += ThirstyAmount;

	if (CurrentThirsty <= 0.0f)
	{
		CurrentThirsty = 0.0f;
	}

	if ( CurrentThirsty >= MaxThirsty )
	{
		CurrentThirsty = MaxThirsty;
	}

	OnThirstChange.ExecuteIfBound(CurrentThirsty, MaxThirsty);
}

void UStatusComponent::AddHunger(float HungerAmount)
{
	CurrentHunger += HungerAmount;

	if ( CurrentHunger <= 0.0f )
	{
		CurrentHunger = 0.0f;
	}

	if ( CurrentHunger >= MaxHunger )
	{
		CurrentHunger = MaxHunger;
	}

	OnHungerChange.ExecuteIfBound(CurrentHunger, MaxHunger);
}