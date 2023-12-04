// Fill out your copyright notice in the Description page of Project Settings.


#include "../Building/Foundation.h"

#include <Kismet/GameplayStatics.h>

#include "Raft.h"

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

ARaft* AFoundation::GetRaft() const
{
	return Raft;
}

void AFoundation::SetRaft(ARaft* NewRaft)
{
	Raft = NewRaft;
}

float AFoundation::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentDurability -= DamageAmount;
	if ( CurrentDurability <= 0.0f )
	{
		Raft->RemoveFoundation(this);
		UGameplayStatics::PlaySoundAtLocation( GetWorld() , AttackSound , GetActorLocation() , GetActorRotation() );
		Destroy();
	}

	return Super::TakeDamage(DamageAmount , DamageEvent , EventInstigator , DamageCauser);
}
