// Fill out your copyright notice in the Description page of Project Settings.


#include "../InGame/InGameWidget.h"

#include "ChargeWidget.h"

void UInGameWidget::Setup()
{

}

void UInGameWidget::SetChargePercent(float NewChargePercent)
{
	ChargeWidget->SetPercent( NewChargePercent );
}