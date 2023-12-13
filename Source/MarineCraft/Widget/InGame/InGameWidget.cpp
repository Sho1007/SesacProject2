// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/InGameWidget.h"

#include "ChargeWidget.h"
#include "InteractWidget.h"
#include "InventoryWidget.h"
#include "ImpactWidget.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"
#include "MarineCraft/Character/StatusComponent.h"
#include "StatusWidget.h"

void UInGameWidget::InitWidget()
{
	UStatusComponent* StatusComponent = Cast<UStatusComponent>(GetOwningPlayerPawn()->GetComponentByClass(UStatusComponent::StaticClass()));
	check(StatusComponent);

	StatusWidget->InitWidget(StatusComponent);
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
	InventoryWidget->UpdateInventory( PlayerInventoryComponent );
}

void UInGameWidget::SetCurrentItem(int32 NewItemIndex)
{
	InventoryWidget->SetCurrentItem( NewItemIndex );
}

void UInGameWidget::ToggleInventory()
{
	InventoryWidget->ToggleInventory();
}

void UInGameWidget::Impact( float DamageAmount )
{
	ImpactWidget->Impact( DamageAmount );
}

void UInGameWidget::ImpactOnDeath()
{
	ImpactWidget->ImpactOnDeath();
}