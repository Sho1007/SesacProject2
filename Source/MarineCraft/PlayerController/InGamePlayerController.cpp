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