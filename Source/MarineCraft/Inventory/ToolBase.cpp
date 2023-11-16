// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ToolBase.h"

#include <Components/BoxComponent.h>

// Sets default values
AToolBase::AToolBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>( TEXT( "BoxComponent" ) );
	SetRootComponent( BoxComponent );

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMeshComponent" ) );
	StaticMeshComponent->SetupAttachment( RootComponent );
	StaticMeshComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );
}

// Called when the game starts or when spawned
void AToolBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}