// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/PlayerInventoryComponent.h"

#include <GameFramework/Character.h>

#include "ItemBase.h"
#include "../MarineCraft.h"
#include "../PlayerController/InGamePlayerController.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	QuickSlot = CreateDefaultSubobject<UInventoryComponent>( TEXT( "QuickSlot" ) );
	QuickSlot->SetInventorySize( 2 );
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentItem( 0 );
}

bool UPlayerInventoryComponent::AddItem(AItemBase* NewItem)
{
	AInGamePlayerController* PC = GetOwner<ACharacter>()->GetController<AInGamePlayerController>();
	check( PC );
	if ( QuickSlot->AddItem( NewItem ) )
	{
		PC->UpdateInventoryWidget( this );
		return true;
	}
	bool Result = Super::AddItem( NewItem );
	PC->UpdateInventoryWidget( this );

	return Result;
}

void UPlayerInventoryComponent::SetCurrentItem(int32 NewItemIndex)
{
	if ( CurrentItemIndex == NewItemIndex ) return;

	if ( NewItemIndex < 0 || 10 <= NewItemIndex ) return;

	//LOG( TEXT( " %d " ) , NewItemIndex );

	if (CurrentItem)
	{
		CurrentItem->SetState( EItemState::InInventory );
	}

	CurrentItemIndex = NewItemIndex;
	CurrentItem = QuickSlot->GetItem( CurrentItemIndex );

	if (CurrentItem)
	{
		CurrentItem->SetState( EItemState::InHand );
		
		LOG( TEXT( "%d Slot : %s" ) , NewItemIndex , *CurrentItem->GetName() );
	}
	else
	{
		LOG( TEXT( "%d Slot is Empty" ) , NewItemIndex);
	}

	// Todo : Update Inventory Widget;
}

UInventoryComponent* UPlayerInventoryComponent::GetQuickSlot() const
{
	return QuickSlot;
}