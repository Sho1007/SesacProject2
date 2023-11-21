// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemSlotWidget.h"

#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>

#include "../../../Inventory/ItemBase.h"
#include "../../../MarineCraftGameInstance.h"



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
		
		Txt_ItemStack->SetText( FText::FromString( FString::FromInt( InstanceData->CurrentStack ) ) );

		UMarineCraftGameInstance* Instance = GetWorld()->GetGameInstance<UMarineCraftGameInstance>();

		check( Instance );

		FItemData* Data = Instance->GetItemData( InstanceData->ItemName );

		check( Data );

		Img_ItemImage->SetBrushFromTexture(Data->ItemImage);
	}
}