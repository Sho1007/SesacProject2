// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemBase.h"

#include <Components/BoxComponent.h>

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>( TEXT( "BoxComponent" ) );
	SetRootComponent( BoxComponent );

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMeshComponent" ) );
	StaticMeshComponent->SetupAttachment( RootComponent );
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGrabbed == false) Floating(DeltaTime);
}

bool AItemBase::IsGrabbed() const
{
	return bIsGrabbed;
}

void AItemBase::Floating(float DeltaTime)
{
	AddActorWorldOffset( FloatingDirection );

	FVector Location = GetActorLocation();
	FloatingRate += DeltaTime * FloatingSpeed;

	if (bIsFloatingUp)
	{
		SetActorLocation( FVector( Location.X , Location.Y , FMath::Lerp( - FloatingHeight , FloatingHeight , FloatingRate ) ) );
		if (FloatingRate >= 1.0f)
		{
			bIsFloatingUp = false;
			FloatingRate = 0.0f;
		}
	}
	else
	{
		SetActorLocation( FVector( Location.X , Location.Y , FMath::Lerp( FloatingHeight , -FloatingHeight , FloatingRate ) ) );
		if ( FloatingRate >= 1.0f )
		{
			bIsFloatingUp = true;
			FloatingRate = 0.0f;
		}
	}
}