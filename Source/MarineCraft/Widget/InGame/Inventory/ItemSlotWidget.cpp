// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemSlotWidget.h"

#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Border.h>
#include <Components/BorderSlot.h>
#include <Components/ProgressBar.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

#include "../../../Inventory/ItemBase.h"
#include "MarineCraft/Inventory/Tool/ToolBase.h"
#include "../InventoryWidget.h"

void UItemSlotWidget::Init(AItemBase* NewItem)
{
	if (NewItem == nullptr)
	{
		CurrentItem = nullptr;
		OL_Item->SetVisibility( ESlateVisibility::Collapsed );
	}
	else
	{
		CurrentItem = NewItem;
		
		OL_Item->SetVisibility( ESlateVisibility::Visible );
		
		FItemData* Data = CurrentItem->GetItemData();
		FItemInstanceData* InstanceData = CurrentItem->GetInstanceData();
		check( Data && InstanceData );

		Img_ItemImage->SetBrushFromTexture(Data->ItemImage);

		if ( Data->ItemType == EItemType::Tools  || Data->ItemType == EItemType::Weapons )
		{
			Txt_ItemStack->SetVisibility( ESlateVisibility::Collapsed );
		}
		else
		{
			Txt_ItemStack->SetVisibility( ESlateVisibility::Visible );
			Txt_ItemStack->SetText( FText::FromString( FString::FromInt( InstanceData->CurrentStack ) ) );	
		}

		if ( Data->bUseDurability )
		{
			AToolBase* ToolBase = Cast<AToolBase>( NewItem );
			check( ToolBase );

			PB_DurabilityBar->SetVisibility( ESlateVisibility::Visible );
			PB_DurabilityBar->SetPercent( ToolBase->GetCurrentDurability() / ToolBase->GetMaxDurability() );
		}
		else
		{
			PB_DurabilityBar->SetVisibility( ESlateVisibility::Collapsed );
		}
	}
}

void UItemSlotWidget::SetInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}

void UItemSlotWidget::Select()
{
	if ( UBorderSlot* BorderSlot = Cast<UBorderSlot>( Brd_OutLine->GetContentSlot() ))
	{
		BorderSlot->SetPadding( FMargin( 2.0f ) );
		//LOG( TEXT( "BorderSlot Content Name : %s" ), *BorderSlot->Content->GetName() );
	}
}

void UItemSlotWidget::Unselect()
{
	if ( UBorderSlot* BorderSlot = Cast<UBorderSlot>( Brd_OutLine->GetContentSlot() ) )
	{
		BorderSlot->SetPadding( FMargin( 0.0f ) );
	}
}

void UItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry , InMouseEvent);

	if (CurrentItem)
	{
		InventoryWidget->SetHoverWidget(CurrentItem->GetItemData());
	}
}

void UItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	InventoryWidget->SetHoverWidget(nullptr);
}

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry , InMouseEvent);
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry , InMouseEvent , OutOperation);
}

void UItemSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent , InOperation);
}

FReply UItemSlotWidget::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	// Todo :
	return FReply::Handled();
}