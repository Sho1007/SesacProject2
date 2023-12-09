// Fill out your copyright notice in the Description page of Project Settings.


#include "../Placeable/Purifier.h"

#include <GameFramework/Character.h>

#include "MarineCraft/Character/CharacterBase.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"

void APurifier::Interact(ACharacter* InteractCharacter)
{
	// 상태에 따라 다른 동작

	// 우선 상호작용 대상의 인벤토리를 가져온 후
	UPlayerInventoryComponent* TargetInventoryComponent = Cast<UPlayerInventoryComponent>( InteractCharacter->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
	check( TargetInventoryComponent );

	// 손에 든 아이템을 가져온다
	AItemBase* Item = TargetInventoryComponent->GetCurrentItem();

	// 1. 연료가 [2]개보다 적고
	// 2. 현재 손에 아이템을 들고 있으며
	// 3. 현재 손에 든 아이템이 [나무 판자]인 경우
	if ( FuelCount < 2 && Item && Item->GetItemData()->ItemName.Compare(TEXT("Plank")) == 0 )
	{
		// 손에 있는 나무 판자를 하나 없애고
		TMap<FName , int32> ItemMap;
		ItemMap.Add( TEXT( "Plank" ) , 1 );
		TargetInventoryComponent->RemoveItems( ItemMap );

		FuelCount++;
		Boil();

		Cast<ACharacterBase>( InteractCharacter )->UpdateInventoryWidget();

		return;
	}

	// 정수가 다 된 상태이고 && 인벤토리 공간이 비어있다면
	if ( bIsPurified && TargetInventoryComponent->HasEmptySpace() )
	{
		// Todo : 깨끗한 물 한 컵을 인벤토리에 추가
		/*AItemBase* CupOfFreshWater = GetWorld()->SpawnActor<AItemBase>();
		InventoryComponent->AddItem();*/

		Cast<ACharacterBase>( InteractCharacter )->UpdateInventoryWidget();

		bIsPurified = false;
		bHasCupOfWater = false;
		// Todo : 물 컵 메쉬 꺼주기

		return;
	}

	// 만약
	// 1. 물이 든 컵이 없고
	// 2. 손에 아이템을 들고 있고
	// 2. 그 아이템의 이름이 물이 든 컵 이라면
	if ( bHasCupOfWater == false && Item && Item->GetItemData()->ItemName.Compare( TEXT( "CupOfWater" ) ) == 0 )
	{
		// 손에 있는 물이 든 컵을 지우고
		TMap<FName , int32> ItemMap;
		ItemMap.Add( TEXT( "CupOfWater" ) , 1 );
		TargetInventoryComponent->RemoveItems( ItemMap );

		Cast<ACharacterBase>( InteractCharacter )->UpdateInventoryWidget();

		// 컵을 가지고 있다고 정하고
		bHasCupOfWater = true;

		CurrentPurifyingTime = 0.0f;

		// Todo : 물 컵 메쉬를 켜준다.

		Boil();
	}
}

FText APurifier::GetInteractActorName( APlayerController* InteractPlayerController )
{
	
	UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>( InteractPlayerController->GetPawn()->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
	check( PlayerInventoryComponent );

	AItemBase* Item = PlayerInventoryComponent->GetCurrentItem();

	if ( bIsPurified )
	{
		return FText::FromString(TEXT("Pick up fresh water"));
	}
	if ( bIsBoiling )
	{
		return FText::FromString( TEXT( "Boiling ..." ) );
	}

	if ( bHasCupOfWater == false && Item && Item->GetItemData()->ItemName.Compare( TEXT( "CupOfWater" ) ) == 0 )
	{
		return FText::FromString( TEXT( "Put cup of water" ) );
	}

	if ( FuelCount < 2 && Item && Item->GetItemData()->ItemName.Compare( TEXT( "Plank" ) ) == 0 )
	{
		return FText::FromString( TEXT( "Put Plank to Fuel" ) );
	}

	return Super::GetInteractActorName( InteractPlayerController );
}

void APurifier::Boil()
{
	// 1. 컵이 있고
	// 2. 정수되지 않았고
	// 3. 최대 정수 시간까지 도달하지 못했다면
	if ( bHasCupOfWater && !bIsPurified && CurrentPurifyingTime < MaxPurifyingTime )
	{
		// 현재 불이 붙어 있는 경우
		if ( CurrentFuelTime > 0.0f )
		{
			bIsBoiling = true;
		}
		// 불이 켜져 있지 않은 상태일 때
		else
		{
			// 남은 연료가 있다면
			if ( FuelCount > 0 )
			{
				// Todo : 불 이펙트 / 연기 이펙트 켜주기
				bIsBoiling = true;

				// 연료를 감소시키고 연료 시간을 증가
				FuelCount--;
				CurrentFuelTime += MaxFuelTime;

				// 틱 켜주기
				SetActorTickEnabled( true );
			}
		}
	}
}

void APurifier::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if ( bIsPlaced == false ) return;

	// 연료 시간을 감소시키고
	CurrentFuelTime -= DeltaSeconds;

	// 만약 끓이는 상태라면
	if ( bIsBoiling )
	{
		// 현재 정수 시간을 증가
		CurrentPurifyingTime += DeltaSeconds;

		// 정수 시간이 다 됐다면
		if ( CurrentPurifyingTime >= MaxPurifyingTime )
		{
			// 끓이는걸 꺼주고
			bIsBoiling = false;
			// 정수됐다고 켜주고
			bIsPurified = true;
		}
	}

	// 만약 연료 시간이 다됐다면
	if ( CurrentFuelTime <= 0.0f )
	{
		// 현재 끓이는 상태였을 경우 && 연료가 아직 있다면
		if ( bIsBoiling && FuelCount > 0 )
		{
			// 연료를 감소시키고
			FuelCount--;
			// 연료 시간을 증가
			CurrentFuelTime += MaxFuelTime;
		}
		// 현재 끓이는 상태가 아니거나 || 연료가 부족하다면
		else
		{
			// 끓이는 상태를 꺼주고
			bIsBoiling = false;
			// Todo : 불 / 연기 이펙트 꺼주기
			// 틱 멈추기
			SetActorTickEnabled( false );
		}
	}
}