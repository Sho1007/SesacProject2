// Fill out your copyright notice in the Description page of Project Settings.


#include "../PlayerController/InGamePlayerController.h"

#include "../Widget/InGame/InGameWidget.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check( InGameWidgetClass );

	InGameWidget = CreateWidget<UInGameWidget>( this , InGameWidgetClass, TEXT("InGameWidget"));
	InGameWidget->AddToViewport();
	//InGameWidget->Setup();
}

void AInGamePlayerController::SetChargePercent(float NewChargePercent)
{
	InGameWidget->SetChargePercent(NewChargePercent);
}

void AInGamePlayerController::UpdateInteractActor(IInteractInterface* NewInteractActor)
{
	InGameWidget->UpdateInteractActor( NewInteractActor );
}

void AInGamePlayerController::UpdateInventoryWidget(UPlayerInventoryComponent* PlayerInventoryComponent)
{
	InGameWidget->UpdateInventory( PlayerInventoryComponent );
}

void AInGamePlayerController::SetCurrentItem(int32 NewItemIndex)
{
	InGameWidget->SetCurrentItem(NewItemIndex);
}

void AInGamePlayerController::ToggleInventory()
{
	InGameWidget->ToggleInventory();
}