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

	// ItemArray의 2차원 배열을 순회하면서
	for (int i = 0; i < ItemArray.Num(); ++i)
	{
		for (int j = 0; j < ItemArray[i].ItemArray.Num(); ++j)
		{
			// 해당 칸에 만약 아이템이 있다면
			if (ItemArray[i].ItemArray[j] != nullptr)
			{
				FItemInstanceData* ExistInstanceData = ItemArray[ i ].ItemArray[ j ]->GetInstanceData();
				FItemData* ExistItemData = ItemArray[ i ].ItemArray[ j ]->GetItemData();
				// 그리고 넣으려는 아이템과 해당 칸에 있는 아이템이 같다면
				if ( ExistItemData->ItemName == NewItemData->ItemName && ExistInstanceData->CurrentStack < ExistItemData->MaxStack )
				{
					// 넣을 수 있는 만큼 넣고
					int32 AddableItemCount = FMath::Min( ExistItemData->MaxStack - ExistInstanceData->CurrentStack, NewInstanceData->CurrentStack);
					NewInstanceData->CurrentStack -= AddableItemCount;
					ExistInstanceData->CurrentStack += AddableItemCount;

					// 만약 다 넣었다면 true 반환
					if ( NewInstanceData->CurrentStack == 0)
					{
						return true;
					}
				}
			}
			// 해당 칸이 비어있다면
			else
			{
				// 해당 칸에 아이템을 넣고 true 반환
				ItemArray[ i ].ItemArray[ j ] = NewItem;
				ItemArray[ i ].ItemArray[ j ]->SetInventoryComponent( this , ( i * 5 ) + j );
				NewItem->AttachToActor( GetOwner() , FAttachmentTransformRules::SnapToTargetNotIncludingScale );

				return true;
			}
		}
	}

	return false;
}

AItemBase* UInventoryComponent::GetItem(int32 ItemIndex)
{
	int32 Row = ItemIndex / 5;
	int32 Col = ItemIndex % 5;

	//LOG( TEXT( "Item Row : %d, Col : %d" ) , Row , Col );

	if (Row < 0 || ItemArray.Num() <= Row || Col < 0 || ItemArray[Row].ItemArray.Num() <= Col )
	{
		LOG( TEXT( "Invalid Index : Inventory Row : %d, Col : %d" ), ItemArray.Num(), ItemArray.Num() > 0 ? ItemArray[0].ItemArray.Num() : 0); 
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
		LOG( TEXT( "Invalid Index : Inventory Row : %d, Col : %d" ) , ItemArray.Num() , ItemArray.Num() > 0 ? ItemArray[ 0 ].ItemArray.Num() : 0 );
		return;
	}

	ItemArray[ Row ].ItemArray[ Col ] = NewItem;
}