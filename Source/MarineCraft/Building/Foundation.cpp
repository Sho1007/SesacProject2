// Fill out your copyright notice in the Description page of Project Settings.


#include "../Building/Foundation.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

#include "Raft.h"

void AFoundation::DoDestroy()
{
	Destroy();
}

void AFoundation::FindAdjacencyFoundation()
{
	TArray<FHitResult> OutHitArray;

	FVector Start = GetActorLocation();
	FVector End;

	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor( this );

	

	End = Start + ( GetActorForwardVector() * 100.0f );
	if ( GetWorld()->LineTraceMultiByChannel( OutHitArray , Start , End , ECC_Visibility , Params ) )
	{
		for (FHitResult& OutHit : OutHitArray)
		{
			if ( AFoundation* Foundation = Cast<AFoundation>(OutHit.GetActor()))
			{
				Foundation->SetAdjacencyFoundation( EDirection::Backward,  this );
				ForwardFoundation = Foundation;
				break;
			}
		}
	}

	End = Start - ( GetActorForwardVector() * 100.0f );
	if ( GetWorld()->LineTraceMultiByChannel( OutHitArray , Start , End , ECC_Visibility , Params ) )
	{
		for ( FHitResult& OutHit : OutHitArray )
		{
			if ( AFoundation* Foundation = Cast<AFoundation>( OutHit.GetActor() ) )
			{
				Foundation->SetAdjacencyFoundation( EDirection::Forward , this );
				BackwardFoundation = Foundation;
				break;
			}
		}
	}

	End = Start + ( GetActorRightVector() * 100.0f );
	if ( GetWorld()->LineTraceMultiByChannel( OutHitArray , Start , End , ECC_Visibility , Params ) )
	{
		for ( FHitResult& OutHit : OutHitArray )
		{
			if ( AFoundation* Foundation = Cast<AFoundation>( OutHit.GetActor() ) )
			{
				Foundation->SetAdjacencyFoundation( EDirection::Left , this );
				RightFoundation = Foundation;
				break;
			}
		}
	}

	End = Start - ( GetActorRightVector() * 100.0f );
	if ( GetWorld()->LineTraceMultiByChannel( OutHitArray , Start , End , ECC_Visibility , Params ) )
	{
		for ( FHitResult& OutHit : OutHitArray )
		{
			if ( AFoundation* Foundation = Cast<AFoundation>( OutHit.GetActor() ) )
			{
				Foundation->SetAdjacencyFoundation( EDirection::Right , this );
				LeftFoundation = Foundation;
				break;
			}
		}
	}
}

int32 AFoundation::GetAdjacencyFoundationCount() const
{
	int32 Result = 0;

	if ( ForwardFoundation != nullptr ) Result++;
	if ( BackwardFoundation != nullptr ) Result++;
	if ( LeftFoundation != nullptr ) Result++;
	if ( RightFoundation != nullptr ) Result++;

	return Result;
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
		Destroy();
	}

	return Super::TakeDamage(DamageAmount , DamageEvent , EventInstigator , DamageCauser);
}

void AFoundation::Destroyed()
{
	// 사방의 기반에 내가 부서진다는 것을 알리기

	UE_LOG( LogTemp , Warning , TEXT( "AFoundation::BeginDestroy" ) );

	if ( Raft ) Raft->RemoveFoundation( this );
	UGameplayStatics::PlaySoundAtLocation( GetWorld() , AttackSound , GetActorLocation() , GetActorRotation() );

	if ( IsValid( ForwardFoundation ) ) ForwardFoundation->SetAdjacencyFoundation( EDirection::Backward , nullptr );
	if ( IsValid( BackwardFoundation ) ) BackwardFoundation->SetAdjacencyFoundation( EDirection::Forward , nullptr );
	if ( IsValid( LeftFoundation ) ) LeftFoundation->SetAdjacencyFoundation( EDirection::Right , nullptr );
	if ( IsValid( RightFoundation ) ) RightFoundation->SetAdjacencyFoundation( EDirection::Left , nullptr );

	Super::Destroyed();
}

void AFoundation::SetAdjacencyFoundation(EDirection Direction, AFoundation* Foundation)
{
	switch (Direction)
	{
	case EDirection::Forward:
		ForwardFoundation = Foundation;
		break;
	case EDirection::Backward:
		BackwardFoundation = Foundation;
		break;
	case EDirection::Left:
		LeftFoundation = Foundation;
		break;
	case EDirection::Right:
		RightFoundation = Foundation;
		break;
	}

	int32  AdjacencyFoundationCount = GetAdjacencyFoundationCount();
}

void AFoundation::GetAdjacencyFoundation(TArray<AFoundation*>& OutArray) const
{
	OutArray.Add( ForwardFoundation );
	OutArray.Add( BackwardFoundation );
	OutArray.Add( LeftFoundation );
	OutArray.Add( RightFoundation );
}

void AFoundation::DestroyOnSeperated()
{
	Raft = nullptr;
	Destroy();
}