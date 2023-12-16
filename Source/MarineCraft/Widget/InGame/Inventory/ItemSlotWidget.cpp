// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemSlotWidget.h"

#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Border.h>
#include <Components/BorderSlot.h>
#include <Components/ProgressBar.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

#include "DragPreviewWidget.h"
#include "DragWidget.h"
#include "../../../Inventory/ItemBase.h"
#include "MarineCraft/Inventory/Tool/ToolBase.h"
#include "../InventoryWidget.h"
#include "MarineCraft/Inventory/InventoryComponent.h"

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

void UItemSlotWidget::SetInventoryWidget(UInventoryWidget* NewInventoryWidget, UInventoryComponent* NewInventoryComponent, int32 NewInventoryIndex)
{
	InventoryWidget = NewInventoryWidget;
	InventoryComponent = NewInventoryComponent;
	InventoryIndex = NewInventoryIndex;
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

UInventoryComponent* UItemSlotWidget::GetInventoryComponent() const
{
	return InventoryComponent;
}

int32 UItemSlotWidget::GetInventoryIndex() const
{
	return InventoryIndex;
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
	Super::NativeOnMouseButtonDown(InGeometry , InMouseEvent);

	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry , InMouseEvent , OutOperation);

	UDragWidget* DragWidget = NewObject<UDragWidget>();
	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	DragWidget->WidgetReference = this;
	DragWidget->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	check(DragPreviewWidgetClass);
	UDragPreviewWidget* DragPreviewWidget = CreateWidget<UDragPreviewWidget>(GetOwningPlayer(), DragPreviewWidgetClass);
	DragPreviewWidget->SetCurrentItem(CurrentItem);
	
	// Todo : DefaultDragVisual 에 Border 배경이 없는 새로운 Widget 만들어서 넣기
	DragWidget->DefaultDragVisual = DragPreviewWidget;
	DragWidget->Pivot = EDragPivot::MouseDown;

	OutOperation = DragWidget;
}

void UItemSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent , InOperation);
}

bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry , InDragDropEvent , InOperation);
	
	if (UDragWidget* DragWidgetResult = Cast<UDragWidget>(InOperation))
	{
		// const FVector2D DragWindowOffset =
		// 	InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		// const FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;

		UDragPreviewWidget* DragPreviewWidget = Cast<UDragPreviewWidget>(DragWidgetResult->DefaultDragVisual);

		UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>(DragWidgetResult->WidgetReference);

		// UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::NativeOnDrop) From : %s %d, To : %s %d"),
		// 	*ItemSlotWidget->GetInventoryComponent()->GetName(), ItemSlotWidget->GetInventoryIndex(),
		// 	*this->InventoryComponent->GetName(), this->InventoryIndex
		// 	);
		ItemSlotWidget->SetVisibility(ESlateVisibility::Visible);
		this->InventoryComponent->GetItemFromInventory(ItemSlotWidget->GetInventoryComponent(), ItemSlotWidget->GetInventoryIndex(), InventoryIndex);

		return true;
	}
	
	return false;
}

FReply UItemSlotWidget::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if (InMouseEvent.GetEffectingButton() == DragKey)
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();

		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();

			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}
	
	return FReply::Handled();
}
