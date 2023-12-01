// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/InventoryWidget.h"

#include <Components/HorizontalBox.h>
#include <Components/Border.h>
#include <Components/VerticalBox.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/UniformGridPanel.h>

#include <GameFramework/Character.h>

#include "Inventory/ItemSlotWidget.h"
#include "Inventory/CraftingListSlotWidget.h"
#include "Inventory/CraftingMaterialSlotWidget.h"
#include "MarineCraft/Inventory/InventoryComponent.h"
#include "Inventory/CategoryButtonWidget.h"
#include "MarineCraft/MarineCraftGameInstance.h"
#include "../../Inventory/ItemBase.h"
#include "Components/Button.h"
#include "Components/VerticalBoxSlot.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

	// Setup QuickSlot

	TArray<UWidget*> ChildWidgetArray = HB_QuickSlot->GetAllChildren();

	for (int i = 0; i < ChildWidgetArray.Num(); ++i)
	{
		if (UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>(ChildWidgetArray[ i ]))
		{
			QuickSlotWidgetArray.Add( ItemSlotWidget );
		}
	}

	// Setup Inventory
	ChildWidgetArray.Empty();
	ChildWidgetArray = UG_Inventory->GetAllChildren();

	for ( int i = 0; i < ChildWidgetArray.Num(); ++i )
	{
		if ( UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>( ChildWidgetArray[ i ] ) )
		{
			InventoryWidgetArray.Add( ItemSlotWidget );
		}
	}

	// Setup Category
	ChildWidgetArray.Empty();
	ChildWidgetArray = VB_CategoryButtonBox->GetAllChildren();

	for ( int i = 0; i < ChildWidgetArray.Num(); ++i )
	{
		if ( UCategoryButtonWidget* CategoryButtonWidget = Cast<UCategoryButtonWidget>( ChildWidgetArray[ i ] ) )
		{
			CategoryButtonWidget->SetInventoryWidget( this );
			if (i == 0)
			{
				SetCraftingList( CategoryButtonWidget->GetCraftingCategoryName() );
			}
		}
	}

	Btn_CraftingButton->OnClicked.AddDynamic( this , &UInventoryWidget::OnCraftingButtonClicked );

	PlayerInventoryComponent = Cast<UPlayerInventoryComponent>( GetOwningPlayerPawn()->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
}

void UInventoryWidget::UpdateQuickSlot( UInventoryComponent* InventoryComponent )
{
	//UE_LOG( LogTemp , Warning , TEXT( "UInventoryWidget::UpdateQuickSlot" ) );
	for (int i = 0; i < QuickSlotWidgetArray.Num(); ++i)
	{
		QuickSlotWidgetArray[ i ]->Init( InventoryComponent->GetItem( i ) );
	}
}

void UInventoryWidget::UpdateInventory(UInventoryComponent* InventoryComponent)
{
	//UE_LOG( LogTemp , Warning , TEXT( "UInventoryWidget::UpdateInventory" ) );
	for ( int i = 0; i < InventoryWidgetArray.Num(); ++i )
	{
		InventoryWidgetArray[ i ]->Init( InventoryComponent->GetItem( i ) );
	}
}

void UInventoryWidget::SetCurrentItem(int32 NewItemIndex)
{
	for ( int i = 0; i < QuickSlotWidgetArray.Num(); ++i )
	{
		if (i == NewItemIndex )
		{
			QuickSlotWidgetArray[ i ]->Select();
		}
		else
		{
			QuickSlotWidgetArray[ i ]->Unselect();
		}
	}
}

void UInventoryWidget::ToggleInventory()
{
	if (Brd_Inventory->GetVisibility() == ESlateVisibility::Visible)
	{
		
		GetOwningPlayer()->SetInputMode( FInputModeGameOnly() );
		GetOwningPlayer()->SetShowMouseCursor( false );
		// Hide
		Brd_Inventory->SetVisibility( ESlateVisibility::Collapsed );
		Brd_Category->SetVisibility( ESlateVisibility::Collapsed );
		Brd_CraftingList->SetVisibility( ESlateVisibility::Collapsed );
		Brd_CraftingItem->SetVisibility( ESlateVisibility::Collapsed );
	}
	else
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus( GetCachedWidget() );
		//InputMode.SetLockMouseToViewportBehavior( EMouseLockMode::LockAlways );
		GetOwningPlayer()->SetInputMode( InputMode );
		GetOwningPlayer()->SetShowMouseCursor( true );
		// Show
		Brd_Inventory->SetVisibility( ESlateVisibility::Visible );
		Brd_Category->SetVisibility( ESlateVisibility::Visible );
		Brd_CraftingList->SetVisibility( ESlateVisibility::Visible );
		Brd_CraftingItem->SetVisibility( ESlateVisibility::Visible );

		UpdateCraftingButton();

		TArray<UWidget*> CraftingMaterialSlotArray = VB_CraftingMaterialBox->GetAllChildren();
		for (int i = 0; i < CraftingMaterialSlotArray.Num(); ++i)
		{
			Cast<UCraftingMaterialSlotWidget>( CraftingMaterialSlotArray[ i ] )->Update();
		}
	}
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab )
	{
		//UE_LOG( LogTemp , Warning , TEXT( "UInventoryWidget::NativeOnKeyDown) Tab Pressed" ) );

		ToggleInventory();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry , InKeyEvent);
}

void UInventoryWidget::SetCraftingList(FName CraftingCategoryName)
{
	UMarineCraftGameInstance* GameInstance = GetWorld()->GetGameInstance<UMarineCraftGameInstance>();
	check( GameInstance );

	FCraftingData* CraftingData = GameInstance->GetCraftingData( CraftingCategoryName );
	Img_CraftingCategoryImage->SetBrushFromTexture( CraftingData->CraftingCategroyImage );
	Txt_CraftingCategoryName->SetText(FText::FromName( CraftingCategoryName ));

	VB_CraftingList->ClearChildren();
	for ( int i = 0; i <  CraftingData->CraftingItemNameArray.Num(); ++i)
	{
		FItemData* ItemData = GameInstance->GetItemData( CraftingData->CraftingItemNameArray[ i ] );
		check( ItemData );

		UCraftingListSlotWidget* CraftingListSlotWidget = CreateWidget<UCraftingListSlotWidget>( GetOwningPlayer() , CraftingListSlotWidgetClass );
		CraftingListSlotWidget->Init( ItemData );
		CraftingListSlotWidget->SetInventoryWidget( this );
		VB_CraftingList->AddChildToVerticalBox( CraftingListSlotWidget )->SetPadding(FMargin(0, 5.0f));
	}
}

void UInventoryWidget::SetCraftingItem(FItemData* NewItemData)
{
	if ( CurrentCraftingItemData == NewItemData ) return;

	CurrentCraftingItemData = NewItemData;

	Img_CraftingItemImage->SetBrushFromTexture( CurrentCraftingItemData->ItemImage );
	Txt_CraftingItemName->SetText( FText::FromName( CurrentCraftingItemData->ItemName ) );
	Txt_CraftingItemDiscription->SetText( CurrentCraftingItemData->ItemDiscription );

	UpdateCraftingButton();
	UpdateCraftingMaterials();
}

void UInventoryWidget::UpdateCraftingButton()
{
	if ( CurrentCraftingItemData == nullptr ) return;
	if ( PlayerInventoryComponent->HasEmptySpace() && PlayerInventoryComponent->CanRemovableItems( CurrentCraftingItemData->CraftingMaterialMap ) )
	{
		Btn_CraftingButton->SetBackgroundColor( ValidButtonColor );
	}
	else
	{
		Btn_CraftingButton->SetBackgroundColor( InvalidButtonColor );
	}
}

void UInventoryWidget::UpdateCraftingMaterials()
{
	if ( CurrentCraftingItemData == nullptr ) return;

	UMarineCraftGameInstance* GameInstance = GetGameInstance<UMarineCraftGameInstance>();
	check( GameInstance );

	VB_CraftingMaterialBox->ClearChildren();

	for ( auto Iter : CurrentCraftingItemData->CraftingMaterialMap )
	{
		FItemData* ItemData = GameInstance->GetItemData( Iter.Key );
		check( ItemData );

		UCraftingMaterialSlotWidget* CraftingMaterialSlotWidget = CreateWidget<UCraftingMaterialSlotWidget>( GetOwningPlayer() , CraftingMaterialSlotWidgetClass );
		VB_CraftingMaterialBox->AddChildToVerticalBox( CraftingMaterialSlotWidget );
		CraftingMaterialSlotWidget->Init( ItemData , Iter.Value );
	}
}

void UInventoryWidget::OnCraftingButtonClicked()
{
	if (Btn_CraftingButton->BackgroundColor == ValidButtonColor )
	{
		PlayerInventoryComponent->RemoveItems( CurrentCraftingItemData->CraftingMaterialMap );

		AItemBase* ItemBase = GetWorld()->SpawnActor<AItemBase>( CurrentCraftingItemData->ItemClass , GetOwningPlayerPawn()->GetActorLocation(), FRotator::ZeroRotator );
		check( ItemBase );

		ItemBase->Interact( Cast<ACharacter>(GetOwningPlayerPawn()) );

		//UE_LOG( LogTemp , Warning , TEXT( "UInventoryWidget::OnCraftingButtonClicked) Valid" ) );

		//  Todo : Update Inventory Not Only QuickSlot
		UpdateQuickSlot( PlayerInventoryComponent->GetQuickSlot() );

		UpdateCraftingButton();
		UpdateCraftingMaterials();
	}
}
