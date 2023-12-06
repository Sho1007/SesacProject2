// Fill out your copyright notice in the Description page of Project Settings.


#include "../Building/Raft.h"

#include <Kismet/KismetSystemLibrary.h>

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
	//UE_LOG( LogTemp , Warning , TEXT( "ARaft::BeginPlay) Root Foundation Name : %s, Root Foundation Label : %s" ), *RootFoundation->GetName(), *RootFoundation->GetActorLabel() );
	AddFoundtaion( Foundation );

	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( FoundationSize , 0, 0) , GetActorRotation());
	AddFoundtaion( Foundation );

	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( FoundationSize , FoundationSize , 0 ) , GetActorRotation() );
	AddFoundtaion( Foundation );

	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( 0 , FoundationSize , 0 ) , GetActorRotation() );
	AddFoundtaion( Foundation );
}

// Called every frame
void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG( LogTemp , Warning , TEXT( "ARaft::Tick) RootLocation : %s" ), *RootLocation.ToString() );

	if ( RootFoundation )
	{
		DrawDebugSphere( GetWorld() , RootFoundation->GetActorLocation() , 30.0f , 10 , FColor::Red );
		DrawDebugSphere( GetWorld() , GetActorLocation() +RootLocation , 30.0f , 10 , FColor::Yellow );
	}

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
	if ( NewFoundation == nullptr || NewFoundation->GetRootComponent() == nullptr ) return;

	RootLocation *= FoundationArray.Num();
	NewFoundation->SetRaft( this );
	NewFoundation->FindAdjacencyFoundation();
	NewFoundation->AttachToActor( this , FAttachmentTransformRules( EAttachmentRule::KeepWorld , EAttachmentRule::KeepRelative , EAttachmentRule::KeepWorld , false ) );
	FoundationArray.Add( NewFoundation );

	RootLocation += NewFoundation->GetRootComponent()->GetRelativeLocation();

	RootLocation /= FoundationArray.Num();

	SetRootFoundation();
	RemoveSeparatedFoundation();
}

void ARaft::RemoveFoundation(AFoundation* NewFoundation)
{
	if ( NewFoundation == nullptr || NewFoundation->GetRootComponent() == nullptr ) return;

	RootLocation *= FoundationArray.Num();

	RootLocation -= NewFoundation->GetRootComponent()->GetRelativeLocation();

	FoundationArray.Remove( NewFoundation );

	RootLocation /= FoundationArray.Num();

	if ( FoundationArray.Num() == 0 )
	{
		Destroy();
	}
	else
	{
		SetRootFoundation();
		RemoveSeparatedFoundation();
	}
}

void ARaft::AddCorner(AFoundation* NewCorner)
{
	CornerSet.Add( NewCorner );
}

void ARaft::RemoveCorner(AFoundation* OldCorner)
{
	//UE_LOG( LogTemp , Warning , TEXT( "ARaft::RemoveCorner) OldCorner : %s" ) , *OldCorner->GetActorLabel() );
	CornerSet.Remove( OldCorner );
}

float ARaft::GetRaftSize() const
{
	if ( CornerSet.Num() == 0 ) return 0.0f;

	float RaftSize = 0.0f;

	for ( AFoundation* Foundation : CornerSet )
	{
		float CurrentSize = ( Foundation->GetActorLocation() - ( GetActorLocation() + RootLocation ) ).Length();

		if ( CurrentSize > RaftSize )
		{
			RaftSize = CurrentSize;
		}
	}

	return RaftSize;
}

void ARaft::PrintRaftSize()
{
	UE_LOG( LogTemp , Warning , TEXT( "ARaft::PrintRaftSize) Raft Size : %f" ), GetRaftSize() );
}

FVector ARaft::GetRootLocation() const
{
	return RootLocation;
}

FVector ARaft::GetMaxDistance() const
{
	return FVector::Zero();
}

TArray<AFoundation*> ARaft::GetCornerArray() const
{
	return CornerSet.Array();
}

void ARaft::SetRootFoundation()
{
	RootFoundation = FoundationArray[ 0 ];
	float MinDistance = ( RootFoundation->GetActorLocation() - ( GetActorLocation() + RootLocation ) ).Length();

	for (AFoundation* Foundation : FoundationArray )
	{
		float CurrentDistance = ( Foundation->GetActorLocation() - ( GetActorLocation() + RootLocation ) ).Length();

		//UE_LOG( LogTemp , Warning , TEXT( "ARaft::SetRootFoundation) MinDistance : %f, %s's Distance : %f" ) , MinDistance, *Foundation->GetActorLabel(), CurrentDistance );
		if (CurrentDistance < MinDistance)
		{
			RootFoundation = Foundation;
			MinDistance = CurrentDistance;
		}
	}
	//UE_LOG( LogTemp , Warning , TEXT( "\nARaft::SetRootFoundation) MinDistance : RootFoundation : %s" ), *RootFoundation->GetActorLabel());
}

void ARaft::RemoveSeparatedFoundation()
{
	TArray<bool> Visited;
	Visited.Init( false , FoundationArray.Num() );

	int32 RootIndex = FoundationArray.Find( RootFoundation );

	TQueue<int32> Queue;
	Visited[ RootIndex ] = true;
	Queue.Enqueue(RootIndex);

	int FoundationCount = 1;
	while (Queue.IsEmpty() == false)
	{
		int32 CurrentIndex = *Queue.Peek(); Queue.Pop();

		TArray<AFoundation*> OutArray;

		FoundationArray[ CurrentIndex ]->GetAdjacencyFoundation( OutArray );

		for (AFoundation* Foundation : OutArray)
		{
			if (IsValid(Foundation))
			{
				int32 FoundationIndex = FoundationArray.Find( Foundation );
				if ( FoundationIndex == -1 || Visited[ FoundationIndex ] ) continue;

				FoundationCount++;
				Visited[ FoundationIndex ] = true;
				Queue.Enqueue( FoundationIndex );
			}
		}

		//UE_LOG( LogTemp , Warning , TEXT( "ARaft::SetRootFoundation) Peek Result : %d" ), CurrentIndex);
	}

	if ( FoundationCount == FoundationArray.Num() ) return;

	//UE_LOG( LogTemp , Warning , TEXT( "ARaft::RemoveSeparatedFoundation) Print Seperated Foundation Name" ) );

	for (int32 i = Visited.Num() - 1; i >= 0 ; --i)
	{
		if ( Visited[ i ] == false )
		{
			AFoundation* DeleteFoundation = FoundationArray[ i ];
			FoundationArray.Remove( DeleteFoundation );
			DeleteFoundation->DestroyOnSeperated();
			//UE_LOG( LogTemp , Warning , TEXT( "ARaft::RemoveSeparatedFoundation) %s" ), *FoundationArray[ i ]->GetActorLabel() );
		}
	}

	// Calculate RootLocation & RootFoundation
	RootLocation = FVector::Zero();

	for ( AFoundation* Foundation : FoundationArray )
	{
		RootLocation += Foundation->GetRootComponent()->GetRelativeLocation();
	}
	RootLocation /= FoundationArray.Num();

	SetRootFoundation();
}