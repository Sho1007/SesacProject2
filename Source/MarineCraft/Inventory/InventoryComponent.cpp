// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	ItemArray.Init( FItemArray() , InventorySize );
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(AItemBase* NewItem)
{
	for (int i = 0; i < ItemArray.Num(); ++i)
	{
		for (int j = 0; j < ItemArray[i].ItemArray.Num(); ++j)
		{
			if (ItemArray[i].ItemArray[j] == nullptr)
			{
				ItemArray[i].ItemArray[j] = NewItem;
			}
		}
	}

	return false;
}