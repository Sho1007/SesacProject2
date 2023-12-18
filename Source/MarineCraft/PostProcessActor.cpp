// Fill out your copyright notice in the Description page of Project Settings.


#include "PostProcessActor.h"

#include "Components/PostProcessComponent.h"

// Sets default values
APostProcessActor::APostProcessActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
}

// Called when the game starts or when spawned
void APostProcessActor::BeginPlay()
{
	Super::BeginPlay();

	
	if (OilMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(OilMaterial, this); 
		PostProcessComponent->AddOrUpdateBlendable(DynamicMaterial, 1.f);
	}
}

// Called every frame
void APostProcessActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OilMaterialWeight > 0.0f)
	{
		OilMaterialWeight = FMath::Max(0.0f, OilMaterialWeight - (DeltaTime * RemoveOilSpeed));
		DynamicMaterial->SetScalarParameterValue("DistortedTexStrength", OilMaterialWeight);
	}
}

void APostProcessActor::SetMaterialWeight(float NewMaterialWeight)
{
	OilMaterialWeight = NewMaterialWeight;
	DynamicMaterial->SetScalarParameterValue("DistortedTexStrength", OilMaterialWeight);
}