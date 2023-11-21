// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/InventoryWidget.h"

#include <Components/HorizontalBox.h>

#include "Inventory/ItemSlotWidget.h"
#include "MarineCraft/Inventory/InventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> ChildWidgetArray = HB_QuickSlot->GetAllChildren();

	for (int i = 0; i < ChildWidgetArray.Num(); ++i)
	{
		if (UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>(ChildWidgetArray[ i ]))
		{
			ItemSlotWidgetArray.Add( ItemSlotWidget );
		}
	}
}

void UInventoryWidget::UpdateQuickSlot( UInventoryComponent* InventoryComponent )
{
	UE_LOG( LogTemp , Warning , TEXT( "UInventoryWidget::UpdateQuickSlot" ) );
	for (int i = 0; i < ItemSlotWidgetArray.Num(); ++i)
	{
		ItemSlotWidgetArray[ i ]->Init( InventoryComponent->GetItem( i ) );
	}
}