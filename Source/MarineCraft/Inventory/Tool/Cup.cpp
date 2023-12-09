// Fill out your copyright notice in the Description page of Project Settings.


#include "../Tool/Cup.h"

#include "MarineCraft/Character/CharacterBase.h"

ACup::ACup()
{
	WaterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "WaterMeshComponent" ) );
	WaterMeshComponent->AttachToComponent( StaticMeshComponent , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
	WaterMeshComponent->SetVisibility( false );
	WaterMeshComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );
}

void ACup::PutWater( bool NewPurified )
{
	bHasWater = true;
	bIsPurified = NewPurified;
	if ( bIsPurified )
	{
		ItemData.ItemName = TEXT( "CupOfFreshWater" );
	}
	else
	{
		ItemData.ItemName = TEXT( "CupOfSaltWater" );
	}

	WaterMeshComponent->SetVisibility( true );

	ItemData.ItemImage = CupOfWaterImage; 

	PlayerCharacter->UpdateInventoryWidget();
}

void ACup::PourWater()
{
	bHasWater = false;
	bIsPurified = false;

	ItemData.ItemName = TEXT( "EmptyCup" );

	WaterMeshComponent->SetVisibility( false );

	ItemData.ItemImage = EmptyCupImage;

	PlayerCharacter->UpdateInventoryWidget();
}

void ACup::Use()
{
	Super::Use();

	// 물이 있다면 -> 마시기
	if (bHasWater)
	{
		// 물을 없애고
		PourWater();

		// 정수가 되었다면
		if ( bIsPurified )
		{
			// Todo : 갈증을 40 채워준다. (+40)
		}
		else
		{
			// Todo : 갈증을 5 고갈시킨다. (-5)
		}
	}
}

void ACup::Cancel()
{
	Super::Cancel();
}