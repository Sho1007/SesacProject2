// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/FloatsamBase.h"

#include <Components/BoxComponent.h>

#include "BlueprintActionFilter.h"

// Sets default values
AFloatsamBase::AFloatsamBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent->SetCollisionProfileName( TEXT( "Floatsam" ) );
}

// Called when the game starts or when spawned
void AFloatsamBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFloatsamBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( bIsGrabbed == false ) Floating( DeltaTime );
}

bool AFloatsamBase::IsGrabbed() const
{
	return bIsGrabbed;
}

void AFloatsamBase::Grab()
{
	bIsGrabbed = true;

	BoxComponent->SetCollisionProfileName( TEXT( "GrabbedFloatsam" ) );
}

void AFloatsamBase::SetFloatingDirection(FVector NewFloatingDirection)
{
	FloatingDirection = NewFloatingDirection;
}

void AFloatsamBase::Floating( float DeltaTime )
{
	// 이동 방향
	AddActorWorldOffset( FloatingDirection );

	FVector Location = GetActorLocation();

	if ( bIsFloatingUp )
	{
		AddActorWorldOffset( FVector( 0 , 0 , FloatingSpeed * DeltaTime ) );
		if ( GetActorLocation().Z >= DefaultHeight + FloatingHeight )
		{
			bIsFloatingUp = false;
		}
	}
	else
	{
		AddActorWorldOffset( FVector( 0 , 0 , -FloatingSpeed * DeltaTime ) );
		if ( GetActorLocation().Z <= DefaultHeight -FloatingHeight )
		{
			bIsFloatingUp = true;
		}
	}
}
