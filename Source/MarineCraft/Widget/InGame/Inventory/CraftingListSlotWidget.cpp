// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/CraftingListSlotWidget.h"

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Border.h>

#include "../InventoryWidget.h"
#include "MarineCraft/Inventory/ItemBase.h"

void UCraftingListSlotWidget::Init( FItemData* NewItemData )
{
	ItemData = NewItemData;

	Img_ItemImage->SetBrushFromTexture( ItemData->ItemImage );
	Txt_ItemName->SetText( FText::FromName(ItemData->ItemName) );
}

void UCraftingListSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry , InMouseEvent);

	//UE_LOG( LogTemp , Warning , TEXT( "UCraftingListSlotWidget::NativeOnMouseEnter" ) );
	Brd_OutLine->SetBrushColor(HoveredColor);
}

void UCraftingListSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	//UE_LOG( LogTemp , Warning , TEXT( "UCraftingListSlotWidget::NativeOnMouseLeave" ) );

	bIsClicked = false;
	Brd_OutLine->SetBrushColor( DefaultColor );
}

FReply UCraftingListSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsClicked = true;
	Brd_OutLine->SetBrushColor( DefaultColor );

	return Super::NativeOnMouseButtonDown(InGeometry , InMouseEvent);
}

FReply UCraftingListSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsClicked)
	{
		InventoryWidget->SetCraftingItem( ItemData );
		Brd_OutLine->SetBrushColor( HoveredColor );
	}

	return Super::NativeOnMouseButtonUp(InGeometry , InMouseEvent);
}

void UCraftingListSlotWidget::SetInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}