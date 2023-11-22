// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/InGameWidget.h"

#include "ChargeWidget.h"
#include "InteractWidget.h"
#include "InventoryWidget.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"

void UInGameWidget::Setup()
{

}

void UInGameWidget::SetChargePercent(float NewChargePercent)
{
	ChargeWidget->SetPercent( NewChargePercent );
}

void UInGameWidget::UpdateInteractActor(IInteractInterface* NewInteractActor)
{
	InteractWidget->UpdateInteractActor( NewInteractActor );
}

void UInGameWidget::UpdateInventory(UPlayerInventoryComponent* PlayerInventoryComponent)
{
	InventoryWidget->UpdateQuickSlot( PlayerInventoryComponent->GetQuickSlot() );
}

void UInGameWidget::SetCurrentItem(int32 NewItemIndex)
{
	InventoryWidget->SetCurrentItem( NewItemIndex );
}