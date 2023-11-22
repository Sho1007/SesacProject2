// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UPlayerInventoryComponent;
class IInteractInterface;
class UInteractWidget;
class UChargeWidget;
class UInventoryWidget;
/**
 * 
 */
UCLASS()
class MARINECRAFT_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void SetChargePercent(float NewChargePercent);
	void UpdateInteractActor(IInteractInterface* NewInteractActor);
	void UpdateInventory(UPlayerInventoryComponent* PlayerInventoryComponent );
	void SetCurrentItem( int32 NewItemIndex );
private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UChargeWidget* ChargeWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UInteractWidget* InteractWidget;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UInventoryWidget* InventoryWidget;
};