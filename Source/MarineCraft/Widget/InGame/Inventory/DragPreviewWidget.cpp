// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/DragPreviewWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MarineCraft/Inventory/ItemBase.h"

void UDragPreviewWidget::SetCurrentItem(AItemBase* NewItem)
{
	CurrentItem = NewItem;
	Img_ItemImage->SetBrushFromTexture(CurrentItem->GetItemData()->ItemImage);
	
	SetMoveStack(CurrentItem->GetInstanceData()->CurrentStack);
}

void UDragPreviewWidget::SetMoveStack(int32 NewMoveStack)
{
	MoveStack = NewMoveStack;
	TB_ItemStack->SetText(FText::FromString(FString::FromInt(CurrentItem->GetInstanceData()->CurrentStack)));
}

FReply UDragPreviewWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry , InKeyEvent);

	UE_LOG(LogTemp, Warning, TEXT("UDragPreviewWidget::NativeOnKeyDown) Key : %s"), *InKeyEvent.GetKey().ToString());

	return FReply::Handled();
}