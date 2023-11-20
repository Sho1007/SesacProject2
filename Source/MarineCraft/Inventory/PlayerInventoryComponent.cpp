// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/PlayerInventoryComponent.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	QuickSlot = CreateDefaultSubobject<UInventoryComponent>( TEXT( "QuickSlot" ) );
	QuickSlot->SetInventorySize( 2 );
}

bool UPlayerInventoryComponent::AddItem(AItemBase* NewItem)
{
	if ( QuickSlot->AddItem( NewItem ) ) return true;

	return Super::AddItem(NewItem);
}