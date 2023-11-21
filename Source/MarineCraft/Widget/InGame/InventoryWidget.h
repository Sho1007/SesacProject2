// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class UInventoryComponent;
class UHorizontalBox;
class UItemSlotWidget;
UCLASS()
class MARINECRAFT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateQuickSlot( UInventoryComponent* InventoryComponent );

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UHorizontalBox* HB_QuickSlot;
	UPROPERTY(Meta = (AllowPrivateAccess))
	TArray<UItemSlotWidget*> ItemSlotWidgetArray;
};