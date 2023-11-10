// Fill out your copyright notice in the Description page of Project Settings.


#include "../Building/BuildingPartsBase.h"

#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
ABuildingPartsBase::ABuildingPartsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABuildingPartsBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABuildingPartsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName ABuildingPartsBase::GetBuildingPartsName(UPrimitiveComponent* TargetComponent) const
{
	for (int i = 0; i < BuildBoxArray.Num(); ++i)
	{
		if (BuildBoxArray[i].BoxArray.Contains(TargetComponent))
		{
			return BuildBoxArray[i].BuildingPartsName;
		}
	}
	return TEXT("");
}