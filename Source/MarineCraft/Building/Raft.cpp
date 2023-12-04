// Fill out your copyright notice in the Description page of Project Settings.


#include "../Building/Raft.h"

#include "Foundation.h"

// Sets default values
ARaft::ARaft()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARaft::BeginPlay()
{
	Super::BeginPlay();

	check( FoundationClass );

	UWorld* World = GetWorld();
	AFoundation* Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() , GetActorRotation() );
	RootFoundation = Foundation;
	//UE_LOG( LogTemp , Warning , TEXT( "ARaft::BeginPlay) Root Foundation Name : %s, Root Foundation Label : %s" ), *RootFoundation->GetName(), *RootFoundation->GetActorLabel() );
	AddFoundtaion( RootFoundation );

	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( FoundationSize , 0, 0) , GetActorRotation());
	AddFoundtaion( Foundation );

	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( FoundationSize , FoundationSize , 0 ) , GetActorRotation() );
	AddFoundtaion( Foundation );

	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( 0 , FoundationSize , 0 ) , GetActorRotation() );
	AddFoundtaion( Foundation );

	//FoundationMap.Remove( Foundation->GetActorLabel() );
}

// Called every frame
void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculate RootLocation
	RootLocation = FVector::ZeroVector;

	int32 FoundationCount = 0;

	for (auto Iter : FoundationMap)
	{
		if (Iter.Value && Iter.Value->GetRootComponent())
		{
			FoundationCount++;
			RootLocation += Iter.Value->GetRootComponent()->GetRelativeLocation();
		}
	}

	RootLocation /= FoundationCount;

	//UE_LOG( LogTemp , Warning , TEXT( "ARaft::Tick) RootLocation : %s" ), *RootLocation.ToString() );

	DrawDebugSphere( GetWorld() , GetActorLocation() +  RootLocation , 30.0f , 10 , FColor::Red);

	// Floating
	if (bFloatingUp)
	{
		if (GetActorLocation().Z >= BaseHeight + FloatingHeight)
		{
			bFloatingUp = false;
		}
		else
		{
			AddActorWorldOffset( FVector( 0 , 0 , DeltaTime * FloatingSpeed ) );
		}
	}
	else
	{
		if ( GetActorLocation().Z <= BaseHeight - FloatingHeight )
		{
			bFloatingUp = true;
		}
		else
		{
			AddActorWorldOffset( FVector( 0 , 0 , -DeltaTime * FloatingSpeed ) );
		}
	}

	AddActorWorldOffset(MoveDirection * MoveSpeed * DeltaTime);
}

void ARaft::AddFoundtaion(AFoundation* NewFoundation)
{
	if ( NewFoundation == false || NewFoundation->GetRootComponent() == false ) return;

	RootLocation *= FoundationMap.Num();

	NewFoundation->SetRaft( this );
	NewFoundation->FindAdjacencyFoundation();
	NewFoundation->AttachToActor( this , FAttachmentTransformRules( EAttachmentRule::KeepWorld , EAttachmentRule::KeepRelative , EAttachmentRule::KeepWorld , false ) );
	FoundationMap.Add( NewFoundation->GetActorLabel() , NewFoundation );

	RootLocation += NewFoundation->GetRootComponent()->GetRelativeLocation();

	RootLocation /= FoundationMap.Num();
}

void ARaft::RemoveFoundation(AFoundation* NewFoundation)
{
	if ( NewFoundation == false || NewFoundation->GetRootComponent() == false ) return;

	RootLocation *= FoundationMap.Num();

	RootLocation -= NewFoundation->GetRootComponent()->GetRelativeLocation();

	FoundationMap.Remove( NewFoundation->GetActorLabel() );

	RootLocation /= FoundationMap.Num();
}

FVector ARaft::GetRootLocation() const
{
	return RootLocation;
}