// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
class AItemBase;
class UOverlay;
class UTextBlock;
class UImage;
class UBorder;
class UProgressBar;
class UInventoryWidget;
UCLASS()
class MARINECRAFT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(AItemBase* NewItem);
	void SetInventoryWidget(UInventoryWidget* NewInventoryWidget);

	void Select();
	void Unselect();
	
protected:
	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;

	// Drag And Drop
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UOverlay* OL_Item;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* Txt_ItemStack;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_ItemImage;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_OutLine;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UProgressBar* PB_DurabilityBar;
	
	//UPROPERTY(Meta = (AllowPrivateAccess))
	UPROPERTY()
	AItemBase* CurrentItem;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly)
	FVector2D DragOffset;
};