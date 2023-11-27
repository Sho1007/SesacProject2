// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "GameFramework/Actor.h"

#include <Engine/DataTable.h>

#include "../Interface/InteractInterface.h"

#include "ItemBase.generated.h"

UENUM()
enum class EItemType : uint8
{
	None,
	Materials,
	Tools,
	Foods,
	Weapons,
	Placeables,
	Equipments,
	Size,
};

UENUM()
enum class EItemState : uint8
{
	None,
	InWorld,
	InInventory,
	InHand,
	Size
};

class AItemBase;
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	FName ItemName;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	int32 MaxStack;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	UTexture2D* ItemImage;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	TSubclassOf<AItemBase> ItemClass;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	EItemType ItemType;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	FText ItemDiscription;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	TMap<FName, int32> CraftingMaterialMap;

	FItemData() : ItemName(TEXT("")), MaxStack(1), ItemImage(nullptr), ItemType()
	{
	}
};

USTRUCT()
struct FCraftingData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly )
	UTexture2D* CraftingCategroyImage;
	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly )
	TArray<FName> CraftingItemNameArray;
};

USTRUCT( BlueprintType )
struct FItemInstanceData
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	int32 CurrentStack;

	FItemInstanceData() : CurrentStack(1)
	{
		
	}
};

class UBoxComponent;
class UStaticMeshComponent;
class UInventoryComponent;
class ACharacterBase;
UCLASS()
class MARINECRAFT_API AItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Getter
	FItemInstanceData* GetInstanceData();
	FItemData* GetItemData();

	// Setter
	void SetState(EItemState NewItemState);
	void SetInventoryComponent( UInventoryComponent* NewInventoryComponent , int32 NewInventoryIndex );
public:
	// Interface
	virtual void Interact( ACharacter* InteractCharacter ) override;
	virtual FText GetInteractActorName() override;

protected:
	// State
	virtual void SetInWorld();
	virtual void SetInInventory();
	virtual void SetInHand();

protected:
	// Inventory
	UPROPERTY( VisibleInstanceOnly )
	ACharacterBase* PlayerCharacter;
	UPROPERTY( VisibleInstanceOnly )
	UInventoryComponent* InventoryComponent;
	UPROPERTY( VisibleInstanceOnly )
	int32 InventoryIndex;
	
	// Item Info
	UPROPERTY(EditAnywhere)
	FItemData ItemData;
	UPROPERTY(EditAnywhere)
	FItemInstanceData InstanceData;
	UPROPERTY(EditAnywhere)
	EItemState State;

	// Component
	UPROPERTY( EditDefaultsOnly )
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY( EditDefaultsOnly )
	UBoxComponent* BoxComponent;
};