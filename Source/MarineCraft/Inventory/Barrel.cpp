// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/Barrel.h"

#include <GameFramework/Character.h>

#include "PlayerInventoryComponent.h"
#include "../MarineCraftGameInstance.h"
#include "MarineCraft/Character/CharacterBase.h"

void ABarrel::Interact(ACharacter* InteractCharacter)
{
	UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>( InteractCharacter->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
	check( PlayerInventoryComponent );

	int LootCount = 4;

	LootCount += FMath::RandRange( 0 , 2 );

	UMarineCraftGameInstance* GameInstance = GetWorld()->GetGameInstance<UMarineCraftGameInstance>();

	for (int i = 0; i < LootCount; ++i )
	{
		float LootChance = FMath::RandRange( 1.0f , 100.0f );
		float TotalLootChance = 0.0f;

		for ( auto Iter : LootItemArray )
		{
			TotalLootChance += Iter.Value;

			if ( LootChance <= TotalLootChance )
			{
				UE_LOG( LogTemp , Warning , TEXT( "ABarrel::Interact) Add Item : %s" ), *Iter.Key.ToString() );
				FItemData* CurrentItemData = GameInstance->GetItemData( Iter.Key );
				if ( CurrentItemData == nullptr ) break;
				AItemBase* Item = GetWorld()->SpawnActor<AItemBase>( CurrentItemData->ItemClass, InteractCharacter->GetActorLocation(), FRotator::ZeroRotator );
				if (PlayerInventoryComponent->AddItem( Item ))
				{
					if (Item->GetInstanceData()->CurrentStack == 0)
					{
						Item->Destroy();
					}
					else
					{
						Item->SetState( EItemState::InInventory );
					}
				}
				else
				{
					Destroy();
				}

				break;
			}
		}
	}



	Destroy();
}
