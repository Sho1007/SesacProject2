// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingListSlotWidget.generated.h"

/**
 * 
 */
struct FItemData;
class UImage;
class UTextBlock;
class UBorder;
class UInventoryWidget;
UCLASS()
class MARINECRAFT_API UCraftingListSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init( FItemData* NewItemData );

	virtual void NativeOnMouseEnter( const FGeometry& InGeometry , const FPointerEvent& InMouseEvent );
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent );

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry , const FPointerEvent& InMouseEvent );
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry , const FPointerEvent& InMouseEvent );

	void SetInventoryWidget( UInventoryWidget* NewInventoryWidget );

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_ItemImage;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* Txt_ItemName;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_OutLine;

	FItemData* ItemData;

	bool bIsClicked;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess) )
	FLinearColor HoveredColor;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	FLinearColor DefaultColor;

	UInventoryWidget* InventoryWidget;
};