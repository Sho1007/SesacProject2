// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/CategoryButtonWidget.h"

#include <Components/Image.h>
#include <Components/Button.h>

#include "MarineCraft/Widget/InGame/InventoryWidget.h"

void UCategoryButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Img_CategoryImage->SetBrushFromTexture( CategoryImage );

	Button->OnClicked.AddDynamic(this, &UCategoryButtonWidget::OnButtonClicked);

	
}

FName UCategoryButtonWidget::GetCraftingCategoryName() const
{
	return CraftingCategoryName;
}

void UCategoryButtonWidget::SetInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}

void UCategoryButtonWidget::OnButtonClicked()
{
	InventoryWidget->SetCraftingList( CraftingCategoryName );
	//UE_LOG( LogTemp , Warning , TEXT( "UCategoryButtonWidget::OnButtonClicked) CategoryName : %s" ), *MyCategoryName.ToString() );
}