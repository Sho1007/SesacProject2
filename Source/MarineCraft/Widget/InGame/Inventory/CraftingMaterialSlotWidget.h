// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingMaterialSlotWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class UBorder;

struct FItemData;
UCLASS()
class MARINECRAFT_API UCraftingMaterialSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(FItemData* NewItemData, int32 NewRequireCount);

	void Update();

private:

	FItemData* ItemData;
	int32 RequireCount;
	int32 CurrentCount;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	FLinearColor DefaultColor;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	FLinearColor InvalidColor;

	UPROPERTY(Meta = (AllowPrivateAccess , BindWidget ))
	UImage* Img_ItemImage;
	UPROPERTY(Meta = (AllowPrivateAccess , BindWidget ))
	UTextBlock* Txt_ItemName;
	UPROPERTY(Meta = (AllowPrivateAccess , BindWidget ))
	UBorder* Brd_Background;

	UPROPERTY(Meta = (AllowPrivateAccess , BindWidget ))
	UTextBlock* Txt_CurrentCount;
	UPROPERTY(Meta = (AllowPrivateAccess , BindWidget ))
	UTextBlock* Txt_RequireCount;
};