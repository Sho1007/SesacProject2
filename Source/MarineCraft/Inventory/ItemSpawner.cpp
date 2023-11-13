// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemSpawner.h"

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
	LOG( TEXT( "" ) );
}