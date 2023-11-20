// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemBase.h"

#include <Components/BoxComponent.h>
#include <GameFramework/Character.h>
#include "../Inventory/InventoryComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>( TEXT( "BoxComponent" ) );
	SetRootComponent( BoxComponent );
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetVisibility( false );

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMeshComponent" ) );
	StaticMeshComponent->SetupAttachment( RootComponent );
	StaticMeshComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FItemInstanceData* AItemBase::GetInstanceData()
{
	return &InstanceData;
}

void AItemBase::SetState(EItemState NewItemState)
{
	State = NewItemState;

	switch ( State )
	{
	case EItemState::InWorld:
		BoxComponent->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
		StaticMeshComponent->SetVisibility( true );
		break;
	case EItemState::InInventory:
		if ( InstanceData.CurrentStack == 0 )
		{
			this->Destroy();
		}
		else
		{
			//AttachToActor( InventoryComponent->GetOwner() , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
			BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
			StaticMeshComponent->SetVisibility( false );
		}
		break;
	case EItemState::InHand:
		break;
	}
}

void AItemBase::Interact( ACharacter* InteractCharacter )
{
	UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>( InteractCharacter->GetComponentByClass( UInventoryComponent::StaticClass() ) );
	check( InventoryComponent );

	if ( InventoryComponent->AddItem(this) )
	{
		SetState( EItemState::InInventory );
	}
}