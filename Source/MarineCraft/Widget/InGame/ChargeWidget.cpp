// Fill out your copyright notice in the Description page of Project Settings.

#include "../InGame/ChargeWidget.h"

#include <Components/Image.h>
#include <Kismet/KismetMaterialLibrary.h>

void UChargeWidget::SetPercent( float NewPercent )
{
	if (RoundProgressBarInstance == nullptr)
	{
		RoundProgressBarInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance( GetWorld() , DynamicMaterialParent );

		Img_ProgressBar->SetBrushFromMaterial( RoundProgressBarInstance );
	}
	RoundProgressBarInstance->SetScalarParameterValue( TEXT( "Percent" ) , NewPercent );
}