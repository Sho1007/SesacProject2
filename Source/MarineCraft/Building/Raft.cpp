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
	Foundation->AttachToActor( this, FAttachmentTransformRules::SnapToTargetNotIncludingScale );
	RootFoundation = Foundation;
	OutLineFoundationArray.Add( Foundation );

	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( FoundationSize , 0, 0) , GetActorRotation());
	Foundation->AttachToActor( this , FAttachmentTransformRules( EAttachmentRule::KeepWorld , EAttachmentRule::KeepRelative , EAttachmentRule::KeepWorld , false ) );
	OutLineFoundationArray.Add( Foundation );
	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( FoundationSize , FoundationSize , 0 ) , GetActorRotation() );
	Foundation->AttachToActor( this , FAttachmentTransformRules( EAttachmentRule::KeepWorld , EAttachmentRule::KeepRelative , EAttachmentRule::KeepWorld , false ) );
	OutLineFoundationArray.Add( Foundation );
	Foundation = World->SpawnActor<AFoundation>( FoundationClass , GetActorLocation() + FVector( 0 , FoundationSize , 0 ) , GetActorRotation() );
	Foundation->AttachToActor( this , FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false) );
	OutLineFoundationArray.Add( Foundation );
}

// Called every frame
void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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