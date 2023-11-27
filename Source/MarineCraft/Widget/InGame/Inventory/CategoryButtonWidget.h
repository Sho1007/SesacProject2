// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CategoryButtonWidget.generated.h"

/**
 * 
 */
class UTexture2D;
class UImage;
class UButton;
class UInventoryWidget;
UCLASS()
class MARINECRAFT_API UCategoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FName GetCraftingCategoryName() const;

	void SetInventoryWidget( UInventoryWidget* NewInventoryWidget );

public:
	UFUNCTION()
	void OnButtonClicked();

private:
	UPROPERTY(Meta = ( AllowPrivateAccess, BindWidget ) )
	UImage* Img_CategoryImage;
	UPROPERTY(Meta = ( AllowPrivateAccess, BindWidget ) )
	UButton* Button;

	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess/*, ExposeOnSpawn = "true" */ ) )
	FName CraftingCategoryName;
	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess/*, ExposeOnSpawn = "true" */ ) )
	UTexture2D* CategoryImage;

	UInventoryWidget* InventoryWidget;
};