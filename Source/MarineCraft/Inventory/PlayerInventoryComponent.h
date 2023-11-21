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

	virtual void BeginPlay() override;

	virtual bool AddItem(AItemBase* NewItem) override;

	void SetCurrentItem(int32 NewItemIndex);

	UInventoryComponent* GetQuickSlot() const;
private:
	UPROPERTY(VisibleInstanceOnly, Meta = ( AllowPrivateAccess ))
	int32 CurrentItemIndex = -1;
	UPROPERTY(VisibleInstanceOnly, Meta = ( AllowPrivateAccess ))
	AItemBase* CurrentItem;

	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess ) )
	UInventoryComponent* QuickSlot;
};