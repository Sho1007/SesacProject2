// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragPreviewWidget.generated.h"

class UTextBlock;
class UImage;
class AItemBase;
/**
 * 
 */
UCLASS()
class MARINECRAFT_API UDragPreviewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCurrentItem(AItemBase* NewItem);
	void SetMoveStack(int32 NewMoveStack);

	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;
	
private:
	int32 MoveStack;
	UPROPERTY()
	AItemBase* CurrentItem;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UImage* Img_ItemImage;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_ItemStack;
};