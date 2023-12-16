// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/InventoryComponent.h"

#include "ItemBase.h"
#include "MarineCraft/MarineCraftGameInstance.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ItemArray.Init( FItemArray() , InventorySize );
}

void UInventoryComponent::SetInventorySize(int32 NewInventorySize)
{
	InventorySize = NewInventorySize;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(AItemBase* NewItem)
{
	FItemInstanceData* NewInstanceData = NewItem->GetInstanceData();
	FItemData* NewItemData = NewItem->GetItemData();

	bool bResult = false;

	// 1차 : Item이 있을 때 먼저 채워넣기
	for ( int i = 0; i < ItemArray.Num(); ++i )
	{
		for ( int j = 0; j < ItemArray[ i ].ItemArray.Num(); ++j )
		{
			// 해당 칸에 만약 아이템이 있다면
			if ( ItemArray[ i ].ItemArray[ j ] != nullptr )
			{
				FItemInstanceData* ExistInstanceData = ItemArray[ i ].ItemArray[ j ]->GetInstanceData();
				FItemData* ExistItemData = ItemArray[ i ].ItemArray[ j ]->GetItemData();
				// 그리고 넣으려는 아이템과 해당 칸에 있는 아이템이 같다면
				if ( ExistItemData->ItemName == NewItemData->ItemName && ExistInstanceData->CurrentStack < ExistItemData->MaxStack )
				{
					// 넣을 수 있는 만큼 넣고
					int32 AddableItemCount = FMath::Min( ExistItemData->MaxStack - ExistInstanceData->CurrentStack , NewInstanceData->CurrentStack );
					NewInstanceData->CurrentStack -= AddableItemCount;
					ExistInstanceData->CurrentStack += AddableItemCount;

					// 만약 다 넣었다면 true 반환
					if ( NewInstanceData->CurrentStack == 0 )
					{
						bResult = true;
						break;
					}
				}
			}
		}
		if (bResult) break;
	}

	if (NewInstanceData->CurrentStack > 0)
	{
		// 2차 빈 곳에 넣기
		for (int i = 0; i < ItemArray.Num(); ++i)
		{
			for (int j = 0; j < ItemArray[i].ItemArray.Num(); ++j)
			{
				if ( ItemArray[ i ].ItemArray[ j ] == nullptr )
				{
					// 해당 칸에 아이템을 넣고 true 반환
					SetItem((i * 5) + j, NewItem);
					bResult = true;
					break;
				}
			}
			if (bResult) break;
		}
	}

	OnInventoryChanged.Broadcast();
	
	return bResult;
}

AItemBase* UInventoryComponent::GetItem(int32 ItemIndex)
{
	int32 Row = ItemIndex / 5;
	int32 Col = ItemIndex % 5;

	//LOG( TEXT( "Item Row : %d, Col : %d" ) , Row , Col );

	if (Row < 0 || ItemArray.Num() <= Row || Col < 0 || ItemArray[Row].ItemArray.Num() <= Col )
	{
		//LOG( TEXT( "Invalid Index : Inventory Row : %d, Col : %d" ), ItemArray.Num(), ItemArray.Num() > 0 ? ItemArray[0].ItemArray.Num() : 0); 
		return nullptr;
	}

	return ItemArray[Row].ItemArray[Col];
}

void UInventoryComponent::SetItem(int32 NewItemIndex, AItemBase* NewItem)
{
	int32 Row = NewItemIndex / 5;
	int32 Col = NewItemIndex % 5;

	if ( Row < 0 || ItemArray.Num() <= Row || Col < 0 || ItemArray[ Row ].ItemArray.Num() <= Col )
	{
		//LOG( TEXT( "Invalid Index : Inventory Row : %d, Col : %d" ) , ItemArray.Num() , ItemArray.Num() > 0 ? ItemArray[ 0 ].ItemArray.Num() : 0 );
		return;
	}

	ItemArray[ Row ].ItemArray[ Col ] = NewItem;
	if (ItemArray[ Row ].ItemArray[ Col ]) ItemArray[ Row ].ItemArray[ Col ]->SetInventoryComponent( this , ( Row * 5 ) + Col );

	OnInventoryChanged.Broadcast();
}

int32 UInventoryComponent::GetItemCount(FName TargetItemName )
{
	int32 Sum = 0;

	for (int i = 0; i < ItemArray.Num(); ++i)
	{
		for (int j = 0; j < ItemArray[i].ItemArray.Num(); ++j)
		{
			if ( ItemArray[ i ].ItemArray[ j ] != nullptr && ItemArray[ i ].ItemArray[ j ]->GetItemData()->ItemName == TargetItemName)
			{
				FItemInstanceData* InstanceData = ItemArray[ i ].ItemArray[ j ]->GetInstanceData();
				Sum += InstanceData->CurrentStack;
			}
		}
	}

	return Sum;
}

bool UInventoryComponent::RemoveItemCount(FName TargetItemName, int32& RemoveCount)
{
	bool bResult = false;
	for ( int i = 0; i < ItemArray.Num(); ++i )
	{
		for ( int j = 0; j < ItemArray[ i ].ItemArray.Num(); ++j )
		{
			if ( ItemArray[ i ].ItemArray[ j ] != nullptr && ItemArray[ i ].ItemArray[ j ]->GetItemData()->ItemName == TargetItemName )
			{
				
				FItemInstanceData* InstanceData = ItemArray[ i ].ItemArray[ j ]->GetInstanceData();
				int32 RemovableCount = FMath::Min( RemoveCount , InstanceData->CurrentStack );
				InstanceData->CurrentStack -= RemovableCount;
				if (InstanceData->CurrentStack == 0)
				{
					ItemArray[ i ].ItemArray[ j ]->Destroy();
					ItemArray[ i ].ItemArray[ j ] = nullptr;
				}

				RemoveCount -= RemovableCount;

				if ( RemoveCount == 0 )
				{
					bResult = true;
					break;
				}
			}
		}
	}

	OnInventoryChanged.Broadcast();

	return bResult;
}

bool UInventoryComponent::HasEmptySpace()
{
	for ( int i = 0; i < ItemArray.Num(); ++i )
	{
		for ( int j = 0; j < ItemArray[ i ].ItemArray.Num(); ++j )
		{
			if ( ItemArray[ i ].ItemArray[ j ] == nullptr ) return true;
		}
	}

	return false;
}

void UInventoryComponent::GetItemFromInventory(UInventoryComponent* FromInventoryComponent, int32 FromInventoryIndex,
	int32 ToInventoryIndex)
{
	// 비어있다면
	if (GetItem(ToInventoryIndex) == nullptr)
	{
		SetItem(ToInventoryIndex, FromInventoryComponent->GetItem(FromInventoryIndex));
		FromInventoryComponent->SetItem(FromInventoryIndex, nullptr);
	}
	// Todo : 비어있지 않다면
	else
	{
	}
}