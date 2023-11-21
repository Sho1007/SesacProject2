// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
class AItemBase;
class UOverlay;
class UTextBlock;
class UImage;
UCLASS()
class MARINECRAFT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(AItemBase* NewItem);

private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UOverlay* OL_Item;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* Txt_ItemStack;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_ItemImage;
};