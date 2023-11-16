// Fill out your copyright notice in the Description page of Project Settings.


#include "../Inventory/Hook.h"

#include <Components/BoxComponent.h>
#include <GameFramework/Character.h>
#include <WaterBodyActor.h>
#include <CableComponent.h>

#include "ItemBase.h"


// Sets default values
AHook::AHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComponent->SetSimulatePhysics( true );
	BoxComponent->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );

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
	if (MoveDirection.Length() <= CatchDistance)
	{
		Catch();
	}
	else
	{
		MoveDirection = FVector( MoveDirection.X , MoveDirection.Y , 0 );
		MoveDirection.Normalize();

		AddActorWorldOffset( MoveDirection * DeltaTime * PullSpeed );
	}
}

void AHook::Catch()
{
	bShouldMovetoPlayer = false;
	this->SetActorHiddenInGame(true);
	BoxComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );

	for (AActor* Iter : FloatsamSet)
	{
		LOG( TEXT( "FloatsamName : %s" ) , *Iter->GetName() );
		Iter->Destroy();
	}
}

void AHook::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LOG( TEXT( "Overlapped Actor : %s" ), *OtherActor->GetName());

	if (AWaterBody* Water = Cast<AWaterBody>(OtherActor))
	{
		//LOG( TEXT( "It's a Water!" ) );
		BoxComponent->SetSimulatePhysics( false );
		BoxComponent->SetWorldRotation( FRotator( -90 , 0 , -90 ) );
	}
	else if (AItemBase* Item = Cast<AItemBase>(OtherActor))
	{
		Item->Grab();
		Item->AttachToActor( this , FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
		FloatsamSet.Add( Item );

		bShouldMovetoPlayer = true;
	}
}

void AHook::Launch()
{
	BoxComponent->SetSimulatePhysics( true );
	FVector LaunchDirection = PlayerCharacter->GetActorForwardVector() + PlayerCharacter->GetActorUpVector();
	LaunchDirection.Normalize();
	BoxComponent->AddForce( LaunchDirection * ForceAmount);
}