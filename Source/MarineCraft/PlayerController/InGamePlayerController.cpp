// Fill out your copyright notice in the Description page of Project Settings.


#include "../PlayerController/InGamePlayerController.h"

#include "../Widget/InGame/InGameWidget.h"
#include "../Widget/GameOverWidget.h"
#include "MarineCraft/Character/CharacterBase.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check( InGameWidgetClass );

	InGameWidget = CreateWidget<UInGameWidget>( this , InGameWidgetClass, TEXT("InGameWidget"));
	InGameWidget->AddToViewport();
	InGameWidget->InitWidget();

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor( false );
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

void AInGamePlayerController::Impact( float DamageAmount )
{

	ACharacterBase* CharacterBase = Cast<ACharacterBase>( GetPawn() );
	

	if (CharacterBase->IsDead())
	{
		InGameWidget->ImpactOnDeath();
	}
	else
	{
		InGameWidget->Impact( DamageAmount );
	}
}

void AInGamePlayerController::Die()
{
	check( GameOverWidgetClass );

	SetInputMode( FInputModeUIOnly() );
	SetShowMouseCursor( true );

	UGameOverWidget* GameOverWidget =  CreateWidget<UGameOverWidget>( this , GameOverWidgetClass );
	GameOverWidget->AddToViewport();

	//SetPause( true );
}