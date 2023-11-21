// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/Hook.h"

#include <Components/BoxComponent.h>
#include <GameFramework/Character.h>
#include <WaterBodyActor.h>
#include <CableComponent.h>

#include "FloatsamBase.h"
#include "PlayerInventoryComponent.h"


// Sets default values
AHook::AHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	BoxComponent->SetCollisionProfileName( TEXT( "Hook" ) );
	BoxComponent->CanCharacterStepUpOn = ECB_No;

	CableComponent = CreateDefaultSubobject<UCableComponent>( TEXT( "CableComponent" ) );
	CableComponent->SetupAttachment( RootComponent );
	CableComponent->EndLocation = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AHook::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic( this , &AHook::OnBoxComponentBeginOverlap );

	PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	check( PlayerCharacter );

	CableComponent->SetAttachEndToComponent( PlayerCharacter->GetMesh() , TEXT( "RopeSocket" ) );
	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("HookSocket") );
	
}

// Called every frame
void AHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( bShouldMovetoPlayer ) MoveToPlayer(DeltaTime);
}

void AHook::MoveToPlayer( float DeltaTime )
{
	FVector MoveDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
	MoveDirection = FVector( MoveDirection.X , MoveDirection.Y , 0 );
	if (MoveDirection.Length() <= CatchDistance)
	{
		Catch();
	}
	else
	{
		MoveDirection.Normalize();

		AddActorWorldOffset( MoveDirection * DeltaTime * PullSpeed );
	}
}

void AHook::Catch()
{
	//bIsThrown = false;
	BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	bShouldMovetoPlayer = false;
	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "HookSocket" ) );
	//BoxComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );

	UPlayerInventoryComponent* InventoryComponent = Cast<UPlayerInventoryComponent>(PlayerCharacter->GetComponentByClass(UPlayerInventoryComponent::StaticClass()));
	check( InventoryComponent );

	for ( AFloatsamBase* Iter : FloatsamSet)
	{
		if ( Iter && InventoryComponent->AddItem( Iter ) )
		{
			Iter->SetState( EItemState::InInventory );
		}
	}
}

void AHook::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if ( bIsThrown == false ) return;
	//LOG( TEXT( "Overlapped Actor : %s" ), *OtherActor->GetName());

	if (AWaterBody* Water = Cast<AWaterBody>(OtherActor))
	{
		//LOG( TEXT( "It's a Water!" ) );
		BoxComponent->SetSimulatePhysics( false );
		BoxComponent->SetWorldRotation( FRotator( -90 , 0 , -90 ) );
	}
	else if (AFloatsamBase* Floatsam = Cast<AFloatsamBase>(OtherActor))
	{
		Floatsam->Grab();
		Floatsam->AttachToActor( this , FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
		FloatsamSet.Add( Floatsam );

		bShouldMovetoPlayer = true;
	}
}

void AHook::Launch()
{
	//bIsThrown = true;
	BoxComponent->SetCollisionProfileName( TEXT( "Hook" ) );
	BoxComponent->SetSimulatePhysics( true );
	FVector LaunchDirection = PlayerCharacter->GetActorForwardVector() + PlayerCharacter->GetActorUpVector();
	LaunchDirection.Normalize();
	BoxComponent->AddForce( LaunchDirection * ForceAmount);
}