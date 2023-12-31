﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItemBase;
USTRUCT(BlueprintType)
struct FItemArray
{
	GENERATED_BODY()

	FItemArray()
	{
		ItemArray.Init( nullptr , 5 );
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AItemBase*> ItemArray;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARINECRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void InitializeComponent() override;

public:
	// Setter
	void SetInventorySize( int32 NewInventorySize );

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool AddItem(AItemBase* NewItem);
	virtual AItemBase* GetItem( int32 ItemIndex );
	void SetItem( int32 NewItemIndex , AItemBase* NewItem );

	virtual int32 GetItemCount( FName TargetItemName );
	virtual bool RemoveItemCount( FName TargetItemName , int32& RemoveCount );

	virtual bool HasEmptySpace();

	void GetItemFromInventory(UInventoryComponent* FromInventoryComponent, int32 FromInventoryIndex, int32 ToInventoryIndex);

protected:
	UPROPERTY(EditAnywhere)
	TArray<FItemArray> ItemArray;
	UPROPERTY(EditAnywhere)
	int32 InventorySize;

public:
	DECLARE_MULTICAST_DELEGATE(FDele_Multi)
	FDele_Multi OnInventoryChanged;
};