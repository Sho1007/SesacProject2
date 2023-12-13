// Fill out your copyright notice in the Description page of Project Settings.


#include "../Placeable/Grill.h"

#include "MarineCraft/MarineCraftGameInstance.h"
#include "MarineCraft/Character/CharacterBase.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"
#include "MarineCraft/Inventory/Food/FoodBase.h"

void AGrill::Interact(ACharacter* InteractCharacter)
{
	// 우선 상호작용 대상의 인벤토리를 가져온 후
	UPlayerInventoryComponent* TargetInventoryComponent = Cast<UPlayerInventoryComponent>( InteractCharacter->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
	check( TargetInventoryComponent );

	// 손에 든 아이템을 가져온다
	AItemBase* Item = TargetInventoryComponent->GetCurrentItem();
	//Super::Interact(InteractCharacter);

	// 1. 연료가 [2]개보다 적고
	// 2. 현재 손에 아이템을 들고 있으며
	// 3. 현재 손에 든 아이템이 [나무 판자]인 경우
	if ( CurrentFuelCount < MaxFuelCount && Item && Item->GetItemData()->ItemName.Compare(TEXT("Plank")) == 0 )
	{
		// 손에 있는 나무 판자를 하나 없애고
		TMap<FName , int32> ItemMap;
		ItemMap.Add( TEXT( "Plank" ) , 1 );
		TargetInventoryComponent->RemoveItems( ItemMap );

		CurrentFuelCount++;
		Cook();
		return;
	}

	// 요리가 다 된 상태이고 && 인벤토리 공간이 비어있다면
	if ( bIsCooked && TargetInventoryComponent->HasEmptySpace() )
	{
		// CurrentFood->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		TargetInventoryComponent->AddItem( CurrentFood );
		CurrentFood = nullptr;
		bIsCooked = false;
		return;
	}

	if ( CurrentFood == nullptr && Item && CookableFoodNameArray.Contains(Item->GetItemData()->ItemName))
	{
		CurrentFood = GetWorld()->SpawnActor<AFoodBase>(Item->GetItemData()->ItemClass);
		TMap<FName, int32> ItemMap;
		ItemMap.Add(Item->GetItemData()->ItemName, 1);
		if (TargetInventoryComponent->RemoveItems(ItemMap) == false)
		{
			CurrentFood->Destroy();
			CurrentFood = nullptr;
			return;
		}
		CurrentFood->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CurrentFood->SetActorRelativeLocation(GrillOffsetMap[CurrentFood->GetItemData()->ItemName]);
		
		// Todo : Set Relative Location, Rotation;

		CurrentCookingTime = 0.0f;
		MaxCookingTime = CurrentFood->GetCookTime();

		Cook();
	}
}

void AGrill::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsPlaced == false) return;

	CurrentFuelTime -= DeltaSeconds;

	if (bIsCooking)
	{
		CurrentCookingTime += DeltaSeconds;

		// Todo : MaxCookingTime은 Food 넣어 줄 때, 정보값 읽어서 Setting 해 줄 것
		if (CurrentCookingTime >= MaxCookingTime)
		{
			// Todo : CurrentFood 를 Food Map 을 참조하여 요리된 음식으로 바꿀 것
			FItemData* NewItemData = GetGameInstance<UMarineCraftGameInstance>()->GetItemData(FoodMap[CurrentFood->GetItemData()->ItemName]);
			check(NewItemData);
			
			AFoodBase* CookedFood = GetWorld()->SpawnActor<AFoodBase>(NewItemData->ItemClass);
			check(CookedFood);

			CookedFood->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			CookedFood->SetActorRelativeLocation(CurrentFood->GetRootComponent()->GetRelativeLocation());
			CookedFood->SetActorRelativeRotation(CurrentFood->GetRootComponent()->GetRelativeRotation());
			
			CurrentFood->Destroy();

			CurrentFood = CookedFood;
			
			bIsCooking = false; 
			bIsCooked = true;
		}
	}

	if ( CurrentFuelTime <= 0.0f )
	{
		if ( bIsCooking && CurrentFuelCount > 0 )
		{
			CurrentFuelCount--;
			CurrentFuelTime += MaxFuelTime;
		}
		else
		{
			bIsCooking = false;
			// Todo : 불 / 연기 이펙트 꺼주기
			SetActorTickEnabled( false );
		}
	}
}

FText AGrill::GetInteractActorName(APlayerController* InteractPlayerController)
{
	UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>( InteractPlayerController->GetPawn()->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
	check( PlayerInventoryComponent );

	AItemBase* Item = PlayerInventoryComponent->GetCurrentItem();

	// 최대 연료 갯수보다 부족한 상태에서 손에 Plank 아이템을 들고 있는 경우
	if ( CurrentFuelCount < MaxFuelCount && Item && Item->GetItemData()->ItemName.Compare(TEXT("Plank")) == 0 )
	{
		return FText::FromString( TEXT( "Put Plank to Fuel" ) );
	}

	// 현재 올려진 음식이 없는 상태에서 손에 조리 가능한 음식을 들고 있는 경우
	if (CurrentFood == nullptr && Item && CookableFoodNameArray.Contains(Item->GetItemData()->ItemName))
	{
		return FText::FromString(FString::Printf(TEXT("Put %s for Cook"), *Item->GetItemData()->ItemName.ToString()));
	}

	if (bIsCooked)
	{
		return FText::FromString(FString::Printf(TEXT("Pick up %s"), *CurrentFood->GetItemData()->ItemName.ToString()));
	}

	if (bIsCooking)
	{
		return FText::FromString( TEXT( "Cooking..." ) );
	}
	
	return Super::GetInteractActorName(InteractPlayerController);
}

void AGrill::Cook()
{
	// 요리를 재개해야할 상황인데,
	if ( CurrentFood && bIsCooked == false && CurrentCookingTime < MaxCookingTime )
	{
		// 불이 붙어 있는 상태에서 음식을 넣었을 경우
		if ( CurrentFuelTime > 0.0f )
		{
			bIsCooking = true;
		}
		else
		{
			if ( CurrentFuelCount > 0 )
			{
				CurrentFuelCount--;
				CurrentFuelTime += MaxFuelTime;
				
				// Todo : 불 / 연기 이펙트 켜주기
				bIsCooking = true;
				
				SetActorTickEnabled( true );
			}
		}
	}
}
