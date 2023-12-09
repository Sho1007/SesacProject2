// Fill out your copyright notice in the Description page of Project Settings.


#include "../Placeable/PlaceableBase.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "Components/BoxComponent.h"
#include "MarineCraft/Building/FloorBase.h"
#include "MarineCraft/Character/CharacterBase.h"

APlaceableBase::APlaceableBase()
{
	BoxComponent->SetCollisionProfileName( TEXT( "Placeable" ) );
}

void APlaceableBase::BeginPlay()
{
	Super::BeginPlay();

	OriginalMaterials = StaticMeshComponent->GetMaterials();
}

void APlaceableBase::SetInHand()
{
	Super::SetInHand();
	bIsPlaced = false;

	UStaticMeshComponent* GhostMesh = PlayerCharacter->GetGhostMeshComponent();
	GhostMesh->SetStaticMesh( StaticMeshComponent->GetStaticMesh() );
	GhostMesh->SetWorldScale3D( FVector( 1.0f , 1.0f , 1.0f ) );
	GhostMesh->SetRelativeRotation( FRotator::ZeroRotator );

	SetActorTickEnabled( true );
	BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	PlayerCharacter->GetGhostMeshComponent()->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
}

void APlaceableBase::SetInInventory()
{
	Super::SetInInventory();
	bIsPlaced = false;


	BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	SetActorTickEnabled( false );
}

void APlaceableBase::SetInWorld()
{
	Super::SetInWorld();

	BoxComponent->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	SetActorTickEnabled( false );
}

void APlaceableBase::Place()
{
	UStaticMeshComponent* GhostMesh = PlayerCharacter->GetGhostMeshComponent();

	if (GhostMesh->GetMaterial( 0 )->GetName().Contains(TEXT("Green")))
	{
		APlaceableBase* Placeable = GetWorld()->SpawnActor<APlaceableBase>( ItemData.ItemClass , GhostMesh->GetComponentLocation() + PlaceOffset, GhostMesh->GetComponentRotation() );

		UE_LOG( LogTemp , Warning , TEXT( " GhostMesh Location : %s, Placeable Location : %s " ) , *GhostMesh->GetComponentLocation().ToString() , *Placeable->GetActorLocation().ToString() );

		Placeable->SetState( EItemState::InWorld );
		Placeable->SetPlaced( PlacedFloor );

		if ( --InstanceData.CurrentStack > 0 )
		{
			PlayerCharacter->UpdateInventoryWidget();
		}
		else
		{
			PlayerCharacter->SetQuickSlotItemNull( InventoryIndex );
			PlayerCharacter->UpdateInventoryWidget();
			PlayerCharacter->GetGhostMeshComponent()->SetVisibility( false );
			Destroy();
		}
	}
	else
	{
		//UE_LOG( LogTemp , Warning , TEXT( "APlaceableBase::Place) Can't Place" ) );
	}
}

void APlaceableBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if ( bIsPlaced ) return;

	// 지상에서 걷고 있을때만 설치 가능하도록 제한
	if ( PlayerCharacter->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking )
	{
		PlayerCharacter->GetGhostMeshComponent()->SetVisibility( false );
		return;
	}

	UE_LOG( LogTemp , Warning , TEXT( "APlaceableBase::Tick" ) );

	FHitResult OutHit;

	FVector Start = PlayerCharacter->GetCameraComponent()->GetComponentLocation();;
	FVector End = Start + (PlayerCharacter->GetCameraComponent()->GetForwardVector() * 700.0f);

	FCollisionQueryParams Params;

	UStaticMeshComponent* GhostMesh = PlayerCharacter->GetGhostMeshComponent();

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params))
	{
		if ( AFloorBase* Floor = Cast<AFloorBase>(OutHit.GetActor()) )
		{
			PlacedFloor = Floor;
			GhostMesh->SetWorldLocation( OutHit.Location + GhostMeshOffset );
			PlayerCharacter->SetGhostMeshMaterial();
			GhostMesh->SetVisibility( true );
		}
		else
		{
			PlacedFloor = nullptr;
			GhostMesh->SetVisibility( false );
		}
	}
	else
	{
		PlacedFloor = nullptr;
		GhostMesh->SetVisibility( false );
	}

	DrawDebugLine( GetWorld() , Start , End , FColor::Yellow );
}

void APlaceableBase::SetPlaced(AFloorBase* NewPlacedFloor)
{
	bIsPlaced = true;
	PlacedFloor = NewPlacedFloor;
	AttachToActor( PlacedFloor , FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld , EAttachmentRule::KeepWorld , false) );
}