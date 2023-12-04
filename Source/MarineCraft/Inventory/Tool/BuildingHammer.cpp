// Fill out your copyright notice in the Description page of Project Settings.


#include "../Tool/BuildingHammer.h"

#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>

#include "../../Character/CharacterBase.h"
#include "../../Building/BuildingPartsBase.h"
#include "MarineCraft/MarineCraftGameInstance.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"
#include "../../Building/Raft.h"
#include "../../Building/Foundation.h"

void ABuildingHammer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if ( bIsReadyToBuild ) TraceBuildParts();
}

void ABuildingHammer::SetInHand()
{
	Super::SetInHand();

	bIsReadyToBuild = true;
	BuildTargetComponent = nullptr;
	SetActorTickEnabled( true );
	StaticMeshComponent->SetVisibility( true );
	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "ToolSocket" ) );
	this->SetActorRelativeRotation( FRotator(0, 90, 180.0f) );
	this->SetActorRelativeLocation( FVector(0, 0, -25.0f) );

	PlayerCharacter->GetGhostMeshComponent()->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
}

void ABuildingHammer::SetInInventory()
{
	Super::SetInInventory();

	SetActorTickEnabled( false );
}

void ABuildingHammer::SetInWorld()
{
	Super::SetInWorld();

	SetActorTickEnabled( false );
}

void ABuildingHammer::Use()
{
	Super::Use();

	if (BuildTargetComponent != nullptr && PlayerCharacter->GetGhostMeshOverlappedActorSet().Num() == 0)
	{
		// Spawn BuildingPartsActor
		UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>( PlayerCharacter->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
		check( PlayerInventoryComponent);

		for (auto Iter : BuildingPartsData->BuildingMaterialMap)
		{
			if ( PlayerInventoryComponent->GetItemCount( Iter.Key ) < Iter.Value ) return;
		}

		for (auto Iter : BuildingPartsData->BuildingMaterialMap)
		{
			int32 RemoveCount = Iter.Value;
			PlayerInventoryComponent->RemoveItemCount( Iter.Key , RemoveCount );
		}

		bIsReadyToBuild = false;
		PlayerCharacter->GetGhostMeshComponent()->SetVisibility( false );

		PlayerCharacter->PlayAnimMontage( BuildMontage );

		ABuildingPartsBase* NewBuildingParts = GetWorld()->SpawnActor<ABuildingPartsBase>( BuildingPartsData->Class , BuildTargetComponent->GetComponentLocation() + BuildingPartsData->SpawnLocationOffset , BuildTargetComponent->GetComponentRotation() );

		UGameplayStatics::PlaySoundAtLocation( GetWorld() , BuildSound , BuildTargetComponent->GetComponentLocation() , FRotator::ZeroRotator );

		// Todo : If is not Foundation Modify Attach
		if ( AFoundation* Foundation = Cast<AFoundation>(NewBuildingParts) )
		{
			PlayerCharacter->GetRaft()->AddFoundtaion( Foundation );
		}
		else
		{
			NewBuildingParts->AttachToActor( PlayerCharacter->GetRaft() , FAttachmentTransformRules( EAttachmentRule::KeepWorld , EAttachmentRule::KeepRelative , EAttachmentRule::KeepWorld , false ) );
		}
		BuildTargetComponent = nullptr;
	}
}

void ABuildingHammer::StopUse()
{
	Super::StopUse();

	bIsReadyToBuild = true;
}

FBuildingPartsData* ABuildingHammer::GetBuildingPartsData() const
{
	return BuildingPartsData;
}

void ABuildingHammer::TraceBuildParts()
{
	UStaticMeshComponent* GhostMeshComponent = PlayerCharacter->GetGhostMeshComponent();

	FHitResult OutHit;

	FVector Start = PlayerCharacter->GetCameraComponent()->GetComponentLocation();
	FVector End = Start + PlayerCharacter->GetCameraComponent()->GetForwardVector() * TraceDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor( this );

	//DrawDebugLine( GetWorld() , Start , End , FColor::Cyan );

	/*FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery( ECC_GameTraceChannel1 );*/

	if ( GetWorld()->LineTraceSingleByChannel( OutHit , Start , End , ECC_Camera , CollisionQueryParams ) )
	{
		//LOG( TEXT( "Hit Actor : %s" ) , *OutHit.GetActor()->GetName() );

		//if ( BuildTargetComponent == OutHit.GetComponent() ) return;

		//LOG( TEXT( "ComponentName : %s" ) , *OutHit.GetComponent()->GetName() );

		if ( ABuildingPartsBase* BuildParts = Cast<ABuildingPartsBase>( OutHit.GetComponent()->GetOwner() ) )
		{
			FName BuildingPartsName = BuildParts->GetBuildingPartsName( OutHit.GetComponent() );
			if ( BuildingPartsName.Compare( TEXT( "None" ) ) != 0 )
			{
				BuildTargetComponent = OutHit.GetComponent();

				UMarineCraftGameInstance* GameInstance = GetGameInstance<UMarineCraftGameInstance>();
				check( GameInstance );
				BuildingPartsData = GameInstance->GetBuildingPartsData( BuildingPartsName );
				check( BuildingPartsData );

				GhostMeshComponent->SetStaticMesh( BuildingPartsData->Mesh );
				GhostMeshComponent->SetWorldRotation( BuildTargetComponent->GetComponentRotation() );
				GhostMeshComponent->SetWorldLocation( BuildTargetComponent->GetComponentLocation() + BuildTargetComponent->GetComponentRotation().RotateVector( BuildingPartsData->MeshLocationOffset ) );
				GhostMeshComponent->SetWorldScale3D( BuildingPartsData->Scale );

				PlayerCharacter->SetGhostMeshMaterial();

				GhostMeshComponent->SetVisibility( true );
				return;
			}
		}
	}

	BuildTargetComponent = nullptr;
	GhostMeshComponent->SetVisibility( false );

}