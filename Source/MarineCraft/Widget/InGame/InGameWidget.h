﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
class UChargeWidget;
UCLASS()
class MARINECRAFT_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

	void Setup();
	
private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UChargeWidget* ChargeWidget;
};