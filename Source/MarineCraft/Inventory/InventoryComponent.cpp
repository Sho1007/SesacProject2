// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/InventoryComponent.h"

#include "GeometryTypes.h"
#include "ItemBase.h"
#include "MarineCraft/MarineCraftGameInstance.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

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
	FItemInstanceData* NewInstance = NewItem->GetInstanceData();
	FItemData* ItemData = GetWorld()->GetGameInstance<UMarineCraftGameInstance>()->GetItemData(NewInstance->ItemName);
	check( ItemData );

	// ItemArray의 2차원 배열을 순회하면서
	for (int i = 0; i < ItemArray.Num(); ++i)
	{
		for (int j = 0; j < ItemArray[i].ItemArray.Num(); ++j)
		{
			// 해당 칸에 만약 아이템이 있다면
			if (ItemArray[i].ItemArray[j] != nullptr)
			{
				// 해당 칸의 아이템 인스턴스 정보를 가져온 뒤
				FItemInstanceData* ExistInstance = ItemArray[ i ].ItemArray[ j ]->GetInstanceData();
				// 그리고 넣으려는 아이템과 해당 칸에 있는 아이템이 같다면
				if ( ExistInstance->ItemName == NewInstance->ItemName && ExistInstance->CurrentStack < ItemData->MaxStack )
				{
					// 넣을 수 있는 만큼 넣고
					int32 AddableItemCount = FMath::Min(ItemData->MaxStack - ExistInstance->CurrentStack, NewInstance->CurrentStack);
					NewInstance->CurrentStack -= AddableItemCount;
					ExistInstance->CurrentStack += AddableItemCount;

					// 만약 다 넣었다면 true 반환
					if (NewInstance->CurrentStack == 0)
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
				NewItem->AttachToActor( GetOwner() , FAttachmentTransformRules::SnapToTargetNotIncludingScale );

				return true;
			}
		}
	}

	return false;
}