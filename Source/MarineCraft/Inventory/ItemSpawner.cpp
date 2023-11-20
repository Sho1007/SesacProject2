﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemSpawner.h"

#include <EngineUtils.h>

#include "FloatsamBase.h"
#include "../Building/Boat.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Find Boat in World
	UWorld* World = GetWorld();
	check( World );
	TActorIterator<AActor> It( World , ABoat::StaticClass() );

	Boat = Cast<ABoat>( *It );

	check( Boat );
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentItemSpawnTime += DeltaTime;

	if (CurrentItemSpawnTime >= ItemSpawnTime)
	{
		CurrentItemSpawnTime -= ItemSpawnTime;

		SpawnItem();
	}
}

void AItemSpawner::SpawnItem()
{
	//LOG( TEXT( "Floatsam Index : %d" ), );

	FVector SpawnLocation = Boat->GetActorLocation();
	SpawnLocation -= OceanCurrentsDirection * SpawnDistance;

	FVector RightVector = FRotationMatrix::MakeFromXZ( ( SpawnLocation - Boat->GetActorLocation() ).GetSafeNormal() , GetActorUpVector() ).GetScaledAxis( EAxis::Y );

	FRotator SpawnRotation = FRotationMatrix::MakeFromX( Boat->GetActorLocation() - SpawnLocation ).Rotator();

	AFloatsamBase* Floatsam = GetWorld()->SpawnActor<AFloatsamBase>( FloatsamClassArray[ FMath::RandRange( 0 , FloatsamClassArray.Num() - 1 ) ], SpawnLocation + RightVector * FMath::RandRange( -SpawnOffset , SpawnOffset ) , SpawnRotation);
	if (Floatsam)
	{
		Floatsam->SetFloatingDirection( OceanCurrentsDirection );
	}
}