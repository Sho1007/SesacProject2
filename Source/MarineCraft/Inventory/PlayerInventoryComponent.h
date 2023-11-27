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

	UFUNCTION(BlueprintCallable)
	virtual bool AddItem(AItemBase* NewItem) override;

	void SetCurrentItem(int32 NewItemIndex);

	AItemBase* GetCurrentItem() const;
	void SetQuickSlotItemNull(int32 ItemIndex);

	UInventoryComponent* GetQuickSlot() const;

	virtual int32 GetItemCount(FName TargetItemName) override;

	virtual void RemoveItemCount(FName TargetItemName, int32& RemoveCount) override;

	bool CanRemovableItems(TMap<FName, int32>& ItemMap);
	void RemoveItems(TMap<FName, int32>& ItemMap);

	virtual bool HasEmptySpace() override;
private:
	UPROPERTY(VisibleInstanceOnly, Meta = ( AllowPrivateAccess ))
	int32 CurrentItemIndex = -1;
	UPROPERTY(VisibleInstanceOnly, Meta = ( AllowPrivateAccess ))
	AItemBase* CurrentItem;

	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess ) )
	UInventoryComponent* QuickSlot;
};