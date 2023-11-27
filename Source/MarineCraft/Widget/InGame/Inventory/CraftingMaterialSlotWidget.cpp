// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/CraftingMaterialSlotWidget.h"

#include <Components/Image.h>
#include <Components/TextBlock.h>

#include "../../../Inventory/ItemBase.h"
#include "Components/Border.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"

void UCraftingMaterialSlotWidget::Init( FItemData* NewItemData , int32 NewRequireCount )
{
	ItemData = NewItemData;
	RequireCount = NewRequireCount;

	Img_ItemImage->SetBrushFromTexture( ItemData->ItemImage );
	Txt_ItemName->SetText( FText::FromName( ItemData->ItemName ) );
	Txt_RequireCount->SetText( FText::FromString( FString::FromInt( RequireCount ) ) );

	Update();
}

void UCraftingMaterialSlotWidget::Update()
{
	UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>( GetOwningPlayerPawn()->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
	check( PlayerInventoryComponent );
	CurrentCount = PlayerInventoryComponent->GetItemCount( ItemData->ItemName );
	Txt_CurrentCount->SetText( FText::FromString( FString::FromInt( CurrentCount ) ) );
	if ( CurrentCount < RequireCount )
	{

		UE_LOG( LogTemp , Warning , TEXT( "UCraftingMaterialSlotWidget::Update) Need More Materials" ) );
		// 재료가 부족할 때
		Img_ItemImage->ColorAndOpacity.A = 0.2f;
		Img_ItemImage->SetColorAndOpacity( Img_ItemImage->ColorAndOpacity );

		Brd_Background->SetBrushColor( InvalidColor );
	}
	else
	{
		UE_LOG( LogTemp , Warning , TEXT( "UCraftingMaterialSlotWidget::Update) Enough Materials" ) );
		// 재료가 충분할 때
		Img_ItemImage->ColorAndOpacity.A = 1.0f;
		Img_ItemImage->SetColorAndOpacity( Img_ItemImage->ColorAndOpacity );

		Brd_Background->SetBrushColor( DefaultColor );
	}
}