// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class UInventoryComponent;
class UHorizontalBox;
class UVerticalBox;
class UItemSlotWidget;
class UBorder;
class UTextBlock;
class UImage;
class UButton;
class UCraftingListSlotWidget;
class UCraftingMaterialSlotWidget;
class UPlayerInventoryComponent;
class UUniformGridPanel;

struct FItemData;
UCLASS()
class MARINECRAFT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateQuickSlot( );
	void UpdateInventory( );

	void SetCurrentItem( int32 NewItemIndex );

	void SetHoverWidget(FItemData* NewItemData);

	void ToggleInventory();

	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry , const FKeyEvent& InKeyEvent );

	void SetCraftingList( FName CraftingCategoryName );
	void SetCraftingItem( FItemData* NewItemData );

	// CraftingButton
	void UpdateCraftingButton();
	void UpdateCraftingMaterials();
	UFUNCTION()
	void OnCraftingButtonClicked();

private:
	UPlayerInventoryComponent* PlayerInventoryComponent;

	// Inventory
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_Inventory;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UUniformGridPanel* UG_Inventory;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_HoveredItemImage;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_HoveredItemName;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_HoveredItemDiscription;

	// Category
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_Category;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UVerticalBox* VB_CategoryButtonBox;

	// CraftingList
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_CraftingList;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* Txt_CraftingCategoryName;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_CraftingCategoryImage;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UVerticalBox* VB_CraftingList;
	UPROPERTY(EditDefaultsOnly, Meta = ( AllowPrivateAccess) )
	TSubclassOf<UCraftingListSlotWidget> CraftingListSlotWidgetClass;

	// CraftingItem
	FItemData* CurrentCraftingItemData;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_CraftingItem;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_CraftingItemImage;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* Txt_CraftingItemName;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* Txt_CraftingItemDiscription;
	UPROPERTY( Meta = ( AllowPrivateAccess , BindWidget ) )
	UButton* Btn_CraftingButton;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UVerticalBox* VB_CraftingMaterialBox;
	UPROPERTY( EditDefaultsOnly, Meta = ( AllowPrivateAccess ) )
	TSubclassOf<UCraftingMaterialSlotWidget> CraftingMaterialSlotWidgetClass;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	FLinearColor ValidButtonColor;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	FLinearColor InvalidButtonColor;

	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHorizontalBox* HB_QuickSlot;
	UPROPERTY(Meta = (AllowPrivateAccess))
	TArray<UItemSlotWidget*> QuickSlotWidgetArray;
	UPROPERTY(Meta = (AllowPrivateAccess))
	TArray<UItemSlotWidget*> InventoryWidgetArray;
};