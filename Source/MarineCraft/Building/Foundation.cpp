// Fill out your copyright notice in the Description page of Project Settings.


#include "../Building/Foundation.h"

void AFoundation::FindAdjacencyFoundation()
{
	UWorld* World = GetWorld();
	FHitResult OutHit;
	FVector Start = GetActorLocation();
	FVector End;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor( this );

	End = GetActorLocation() + GetActorForwardVector() * LineTraceDistance;
	if (World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params))
	{
		UE_LOG( LogTemp , Warning , TEXT( "AFoundation::FindAdjacencyFoundation) Forward : %s" ), *OutHit.GetActor()->GetActorLabel() );
	}

	End = GetActorLocation() + GetActorForwardVector() * LineTraceDistance;
	if ( World->LineTraceSingleByChannel( OutHit , Start , End , ECC_Visibility , Params ) )
	{
		UE_LOG( LogTemp , Warning , TEXT( "AFoundation::FindAdjacencyFoundation) Forward : %s" ) , *OutHit.GetActor()->GetActorLabel() );
	}
}