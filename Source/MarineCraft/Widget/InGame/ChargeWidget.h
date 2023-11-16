// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChargeWidget.generated.h"

/**
 * 
 */
class UImage;
UCLASS()
class MARINECRAFT_API UChargeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetPercent(float NewPercent);

private:
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess , BindWidget ) )
	UImage* Img_ProgressBar;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	UMaterialInterface* DynamicMaterialParent;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	UMaterialInstanceDynamic* RoundProgressBarInstance;
};