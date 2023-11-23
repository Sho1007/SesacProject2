﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/PlayerInventoryComponent.h"

#include <GameFramework/Character.h>

#include "ItemBase.h"
#include "../MarineCraft.h"
#include "../PlayerController/InGamePlayerController.h"
#include "../MarineCraftGameInstance.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	QuickSlot = CreateDefaultSubobject<UInventoryComponent>( TEXT( "QuickSlot" ) );
	QuickSlot->SetInventorySize( 2 );
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UMarineCraftGameInstance* GameInstance = GetWorld()->GetGameInstance<UMarineCraftGameInstance>();

	check( GameInstance );

	// Hook
	{
		FItemData* ItemData = GameInstance->GetItemData( "Hook" );

		check( ItemData );

		AItemBase* Hook = GetWorld()->SpawnActor<AItemBase>( ItemData->ItemClass );

		check( Hook );

		AddItem( Hook );
	}
	// Building Hammer
	{
		FItemData* ItemData = GameInstance->GetItemData( "BuildingHammer" );

		check( ItemData );

		AItemBase* BuildingHammer = GetWorld()->SpawnActor<AItemBase>( ItemData->ItemClass );

		check( BuildingHammer );

		AddItem( BuildingHammer );
	}
	

	SetCurrentItem( 0 );
}

bool UPlayerInventoryComponent::AddItem(AItemBase* NewItem)
{
	AInGamePlayerController* PC = GetOwner<ACharacter>()->GetController<AInGamePlayerController>();
	check( PC );
	if ( QuickSlot->AddItem( NewItem ) )
	{
		PC->UpdateInventoryWidget( this );
		NewItem->SetState( EItemState::InInventory );

		return true;
	}
	if ( Super::AddItem( NewItem ))
	{
		PC->UpdateInventoryWidget( this );
		NewItem->SetState( EItemState::InInventory );

		return true;
	}

	return false;
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

	AInGamePlayerController* PC = GetOwner<ACharacter>()->GetController<AInGamePlayerController>();
	check( PC );
	PC->SetCurrentItem( NewItemIndex );
	

	// Todo : Update Inventory Widget;
}

AItemBase* UPlayerInventoryComponent::GetCurrentItem() const
{
	return QuickSlot->GetItem( CurrentItemIndex );
}

void UPlayerInventoryComponent::SetQuickSlotItemNull(int32 ItemIndex)
{
	QuickSlot->SetItem( ItemIndex , nullptr );
}

UInventoryComponent* UPlayerInventoryComponent::GetQuickSlot() const
{
	return QuickSlot;
}

int32 UPlayerInventoryComponent::GetItemCount(FName TargetItemName)
{
	int32 Sum = 0;

	Sum += QuickSlot->GetItemCount( TargetItemName );
	Sum += Super::GetItemCount( TargetItemName );

	return Sum;
}

void UPlayerInventoryComponent::RemoveItemCount(FName TargetItemName, int32& RemoveCount)
{
	QuickSlot->RemoveItemCount( TargetItemName , RemoveCount);

	if (RemoveCount > 0)
	{
		Super::RemoveItemCount( TargetItemName , RemoveCount );
	}

	AInGamePlayerController* PC = GetOwner<ACharacter>()->GetController<AInGamePlayerController>();
	check( PC );
	PC->UpdateInventoryWidget(this);
}