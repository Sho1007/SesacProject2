// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UVerticalBox;
class IInteractInterface;
UCLASS()
class MARINECRAFT_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateInteractActor( IInteractInterface* NewInteractActor);
	
private:
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* Txt_InteractActorName;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UVerticalBox* VB_InteractBox;
};