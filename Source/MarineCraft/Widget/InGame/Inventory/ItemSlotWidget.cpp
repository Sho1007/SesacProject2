// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemSlotWidget.h"

#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Border.h>
#include <Components/BorderSlot.h>
#include <Components/ProgressBar.h>

#include "../../../Inventory/ItemBase.h"
#include "MarineCraft/Inventory/Tool/ToolBase.h"


void UItemSlotWidget::Init(AItemBase* NewItem)
{
	if (NewItem == nullptr)
	{
		OL_Item->SetVisibility( ESlateVisibility::Collapsed );
	}
	else
	{
		OL_Item->SetVisibility( ESlateVisibility::Visible );
		FItemInstanceData* InstanceData = NewItem->GetInstanceData();
		
		

		FItemData* Data = NewItem->GetItemData();

		check( Data );

		Img_ItemImage->SetBrushFromTexture(Data->ItemImage);

		if ( Data->ItemType == EItemType::Tools )
		{
			PB_DurabilityBar->SetVisibility(ESlateVisibility::Visible);
			AToolBase* ToolBase = Cast<AToolBase>(NewItem);
			check( ToolBase );

			PB_DurabilityBar->SetVisibility( ESlateVisibility::Visible );
			PB_DurabilityBar->SetPercent( ToolBase->GetCurrentDurability() / ToolBase->GetMaxDurability() );

			Txt_ItemStack->SetVisibility( ESlateVisibility::Collapsed );
		}
		else
		{
			Txt_ItemStack->SetVisibility( ESlateVisibility::Visible );
			Txt_ItemStack->SetText( FText::FromString( FString::FromInt( InstanceData->CurrentStack ) ) );
			PB_DurabilityBar->SetVisibility( ESlateVisibility::Collapsed );
		}
	}
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