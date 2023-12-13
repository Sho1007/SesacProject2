// Fill out your copyright notice in the Description page of Project Settings.


#include "../Tool/Cup.h"

#include <Camera/CameraComponent.h>

#include "MarineCraft/Character/CharacterBase.h"
#include "MarineCraft/Character/StatusComponent.h"

ACup::ACup()
{
	WaterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "WaterMeshComponent" ) );
	WaterMeshComponent->SetupAttachment(StaticMeshComponent);
	//WaterMeshComponent->AttachToComponent( StaticMeshComponent , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
	WaterMeshComponent->SetVisibility( false );
	WaterMeshComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );
}

void ACup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult OutHit;

	FVector Start = PlayerCharacter->GetCameraComponent()->GetComponentLocation();
	FVector End = Start + PlayerCharacter->GetCameraComponent()->GetForwardVector() * 700.0f;

	FCollisionQueryParams CollisionQueryParams;
	// CollisionQueryParams.AddIgnoredActor( this );

	DrawDebugLine( GetWorld() , Start , End , FColor::Blue );
	
	if ( GetWorld()->LineTraceSingleByChannel( OutHit , Start , End , ECC_GameTraceChannel4 , CollisionQueryParams ) )
	{
		// UE_LOG( LogTemp , Warning , TEXT( "ACup::Tick) OutHit Actor : %s" ), *OutHit.GetActor()->GetActorLabel() );
		WaterActor = OutHit.GetActor();
	}
	else
	{
		WaterActor = nullptr;
	}
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

	SetActorTickEnabled( false );

	if ( PlayerCharacter ) PlayerCharacter->UpdateInventoryWidget();
}

void ACup::PourWater()
{
	bHasWater = false;
	bIsPurified = false;

	ItemData.ItemName = TEXT( "EmptyCup" );

	WaterMeshComponent->SetVisibility( false );

	ItemData.ItemImage = EmptyCupImage;

	SetActorTickEnabled( true );

	if ( PlayerCharacter ) PlayerCharacter->UpdateInventoryWidget();
}

void ACup::Use()
{
	Super::Use();

	// 물이 있다면 -> 마시기
	if (bHasWater)
	{
		// 정수가 되었다면
		if ( bIsPurified )
		{
			// Todo : 갈증을 40 채워준다. (+40)
			// UE_LOG( LogTemp , Warning , TEXT( "ACup::Use) Add 40 Thirsty Guage" ) );
			Cast<UStatusComponent>( PlayerCharacter->GetComponentByClass( UStatusComponent::StaticClass() ) )->AddThirsty(40.0f);
		}
		else
		{
			// Todo : 갈증을 5 고갈시킨다. (-5)
			// UE_LOG( LogTemp , Warning , TEXT( "ACup::Use) Sub 5 Thirsty Guage" ) );
			Cast<UStatusComponent>( PlayerCharacter->GetComponentByClass( UStatusComponent::StaticClass() ) )->AddThirsty( -5.0f );
		}

		// 물을 없앤다
		PourWater();
	}
	// 물이 없다면 -> 물 긷기	
	else
	{
		if ( WaterActor )
		{
			// Todo : 만약 한정된 물 자원이라면 물 양 빼주기
			// WaterActor->RemoveWater();

			// Todo : 만약 이미 정수된 물이라면 true 로 바꿔주기 (아직은 정수된 물 자원이 없음)
			PutWater( false );
		}
	}
}

void ACup::Cancel()
{
	Super::Cancel();
}

void ACup::SetInHand()
{
	Super::SetInHand();

	StaticMeshComponent->SetVisibility( true );

	if ( bHasWater )
	{
		WaterMeshComponent->SetVisibility( true );
	}
	else
	{
		SetActorTickEnabled( true );
	}

	AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "ToolSocket" ) );
	SetActorRelativeLocation( FVector( -3 , -3 , 0 ) );
	SetActorRelativeRotation( FRotator( 0 , 0 , -180 ) );
}

void ACup::SetInInventory()
{
	Super::SetInInventory();

	WaterMeshComponent->SetVisibility( false );
	SetActorTickEnabled( false );
}

void ACup::SetInWorld()
{
	Super::SetInWorld();

	WaterMeshComponent->SetVisibility( false );
	SetActorTickEnabled( false );
}