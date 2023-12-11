// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/InteractWidget.h"

#include <Components/TextBlock.h>
#include <Components/VerticalBox.h>

#include "../../Interface/InteractInterface.h"

void UInteractWidget::UpdateInteractActor( IInteractInterface* NewInteractActor )
{
	if (NewInteractActor)
	{
		VB_InteractBox->SetVisibility( ESlateVisibility::Visible );
		Txt_InteractActorName->SetText( NewInteractActor->GetInteractActorName( GetOwningPlayer() ) );
	}
	else
	{
		VB_InteractBox->SetVisibility( ESlateVisibility::Collapsed );
	}
}