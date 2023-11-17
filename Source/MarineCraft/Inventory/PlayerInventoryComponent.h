// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Inventory/InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

class AItemBase;
UCLASS()
class MARINECRAFT_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	UPlayerInventoryComponent();
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TArray<AItemBase*> QuickSlot;
};