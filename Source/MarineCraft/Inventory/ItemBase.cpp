// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/ItemBase.h"

#include <Components/BoxComponent.h>
#include "../Character/CharacterBase.h"
#include "../Inventory/PlayerInventoryComponent.h"

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

	SetActorTickEnabled( false );
}

FItemInstanceData* AItemBase::GetInstanceData()
{
	return &InstanceData;
}

FItemData* AItemBase::GetItemData()
{
	return &ItemData;
}

void AItemBase::SetState(EItemState NewItemState)
{
	State = NewItemState;

	switch ( State )
	{
	case EItemState::InWorld:
		SetInWorld();
		break;
	case EItemState::InInventory:
		SetInInventory();
		break;
	case EItemState::InHand:
		SetInHand();
		break;
	}
}

void AItemBase::SetInventoryComponent(UInventoryComponent* NewInventoryComponent, int32 NewInventoryIndex)
{
	InventoryComponent = NewInventoryComponent;
	InventoryIndex = NewInventoryIndex;

	PlayerCharacter = Cast<ACharacterBase>(InventoryComponent->GetOwner());
}

void AItemBase::Interact( ACharacter* InteractCharacter )
{
	UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>( InteractCharacter->GetComponentByClass( UPlayerInventoryComponent::StaticClass() ) );
	check( PlayerInventoryComponent );

	if ( PlayerInventoryComponent->AddItem(this) )
	{
		SetState( EItemState::InInventory );
	}
}

FText AItemBase::GetInteractActorName( APlayerController* InteractPlayerController )
{
	return FText::FromName( ItemData.ItemName );
}

void AItemBase::SetInWorld()
{
	BoxComponent->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	StaticMeshComponent->SetVisibility( true );
}

void AItemBase::SetInInventory()
{
	if ( InstanceData.CurrentStack == 0 )
	{
		this->Destroy();
	}
	else
	{
		SetActorTickEnabled( false );
		//AttachToActor( InventoryComponent->GetOwner() , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
		BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
		StaticMeshComponent->SetVisibility( false );
	}
}

void AItemBase::SetInHand()
{
}