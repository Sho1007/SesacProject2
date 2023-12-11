// Fill out your copyright notice in the Description page of Project Settings.


#include "../Tool/Cup.h"

void ACup::PutWater( bool NewPurified )
{
	bHasWater = true;
	bIsPurified = NewPurified;
	if ( bIsPurified )
	{
		ItemData.ItemName = TEXT( "CupOfFreshWater" );
		// Todo : Change Image;
	}
	else
	{
		ItemData.ItemName = TEXT( "CupOfWater" );
		// Todo : Change Image;
	}
}

void ACup::PourWater()
{
	bHasWater = false;
	bIsPurified = false;

	ItemData.ItemName = TEXT( "EmptyCup" );

	// Todo : Change Image
}