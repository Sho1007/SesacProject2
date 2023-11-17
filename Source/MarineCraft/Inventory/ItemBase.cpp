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
	BoxComponent->CanCharacterStepUpOn = ECB_No;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMeshComponent" ) );
	StaticMeshComponent->SetupAttachment( RootComponent );
	StaticMeshComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
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
}