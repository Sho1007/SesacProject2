// Fill out your copyright notice in the Description page of Project Settings.


#include "../Tool/BuildingHammer.h"

#include <Camera/CameraComponent.h>

#include "../../Character/CharacterBase.h"
#include "../../Building/BuildingPartsBase.h"
#include "MarineCraft/MarineCraftGameInstance.h"
#include "MarineCraft/Inventory/PlayerInventoryComponent.h"
#include "../../Building/Raft.h"


void ABuildingHammer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UStaticMeshComponent* GhostMeshComponent = PlayerCharacter->GetGhostMeshComponent();

	FHitResult OutHit;

	FVector Start = PlayerCharacter->GetCameraComponent()->GetComponentLocation();
	FVector End = Start + PlayerCharacter->GetCameraComponent()->GetForwardVector() * TraceDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor( this );

	DrawDebugLine( GetWorld() , Start , End , FColor::Cyan );
	if ( GetWorld()->LineTraceSingleByChannel( OutHit , Start , End , ECC_Visibility , CollisionQueryParams ) )
	{
		LOG( TEXT( "Hit Actor : %s" ) , *OutHit.GetActor()->GetName() );

		//if ( BuildTargetComponent == OutHit.GetComponent() ) return;

		LOG( TEXT( "ComponentName : %s" ) , *OutHit.GetComponent()->GetName() );

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

				GhostMeshComponent->SetStaticMesh(BuildingPartsData->Mesh);
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

void ABuildingHammer::SetInHand()
{
	Super::SetInHand();

	BuildTargetComponent = nullptr;
	SetActorTickEnabled( true );
	StaticMeshComponent->SetVisibility( true );
	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "HammerSocket" ) );

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

	//LOG( TEXT( "" ) );
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

		PlayerCharacter->PlayAnimMontage( BuildMontage );

		ABuildingPartsBase* NewBuildingParts = GetWorld()->SpawnActor<ABuildingPartsBase>( BuildingPartsData->Class , BuildTargetComponent->GetComponentLocation() + BuildingPartsData->SpawnLocationOffset , BuildTargetComponent->GetComponentRotation() );
		NewBuildingParts->AttachToActor( PlayerCharacter->GetRaft() , FAttachmentTransformRules( EAttachmentRule::KeepWorld , EAttachmentRule::KeepRelative , EAttachmentRule::KeepWorld , false ) );
		BuildTargetComponent = nullptr;
		PlayerCharacter->GetGhostMeshComponent()->SetVisibility( false );
	}
}

FBuildingPartsData* ABuildingHammer::GetBuildingPartsData() const
{
	return BuildingPartsData;
}