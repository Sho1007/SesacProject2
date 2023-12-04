// Fill out your copyright notice in the Description page of Project Settings.


#include "../Tool/Hook.h"

#include <Components/BoxComponent.h>
#include <GameFramework/Character.h>
#include <WaterBodyActor.h>
#include <CableComponent.h>
#include <Kismet/GameplayStatics.h>

#include "../FloatsamBase.h"
#include "../PlayerInventoryComponent.h"
#include "MarineCraft/PlayerController/InGamePlayerController.h"
#include "../../Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AHook::AHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	BoxComponent->SetCollisionProfileName( TEXT( "Hook" ) );
	BoxComponent->CanCharacterStepUpOn = ECB_No;

	CableComponent = CreateDefaultSubobject<UCableComponent>( TEXT( "CableComponent" ) );
	CableComponent->SetupAttachment( RootComponent );
	CableComponent->EndLocation = FVector::ZeroVector;

	RopeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "RopeMeshComponent" ) );
	RopeMeshComponent->SetCollisionProfileName( TEXT( "NoCollision" ) );
	RopeMeshComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

// Called when the game starts or when spawned
void AHook::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic( this , &AHook::OnBoxComponentBeginOverlap );

	PlayerCharacter = Cast<ACharacterBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check( PlayerCharacter );

	CableComponent->SetAttachEndToComponent( PlayerCharacter->GetMesh() , TEXT( "RopeSocket" ) );
	RopeMeshComponent->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "RopeSocket" ) );
	RopeMeshComponent->SetRelativeLocationAndRotation( FVector( 3 , 0 , -12 ) , FRotator( 90 , 0 , 0 ) );
}

// Called every frame
void AHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG( LogTemp , Warning , TEXT( "AHook::Tick) Outer : %s" ), *GetOwner()->GetName() );

	if ( bShouldMovetoPlayer ) MoveToPlayer(DeltaTime);
	if ( bIsCharging )
	{
		CurrentChargeTime += DeltaTime;
		
		if ( CurrentChargeTime >= MaxChargeTime )
		{
			StopUse();
		}
		else
		{
			AInGamePlayerController* PC = PlayerCharacter->GetController<AInGamePlayerController>();
			check( PC );
			PC->SetChargePercent( CurrentChargeTime / MaxChargeTime );
		}
	}
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
	BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	bShouldMovetoPlayer = false;
	bIsLanded = false;
	bIsThrown = false;
	SetInHand();

	UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>(PlayerCharacter->GetComponentByClass(UPlayerInventoryComponent::StaticClass()));
	check( PlayerInventoryComponent );

	int32 FloatsamCount = FloatsamSet.Num();

	if (FloatsamCount > 0)
	{
		CurrentDurability = FMath::Max(0.0f, CurrentDurability - (float)FloatsamCount);

		for ( AFloatsamBase* Iter : FloatsamSet )
		{
			if ( Iter )
			{
				Iter->Interact( Cast<ACharacter>(PlayerInventoryComponent->GetOwner()) );
			}
		}

		FloatsamSet.Empty();

		if (CurrentDurability == 0.0f)
		{
			PlayerInventoryComponent->SetQuickSlotItemNull( InventoryIndex );
			Destroy();
		}

		PlayerCharacter->UpdateInventoryWidget();
	}
}

void AHook::SetInHand()
{
	Super::SetInHand();

	SetActorTickEnabled( true );

	StaticMeshComponent->SetVisibility( true );
	CableComponent->SetVisibility( true );
	RopeMeshComponent->SetVisibility( true );
	BoxComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "ToolSocket" ) );
	this->SetActorRelativeLocation( FVector(-2, 0, -18) );
	this->SetActorRelativeRotation( FRotator(0, 180, 0) );
}

void AHook::SetInInventory()
{
	SetActorTickEnabled( false );

	BoxComponent->SetSimulatePhysics( false );
	
	CableComponent->SetVisibility( false );
	RopeMeshComponent->SetVisibility( false );
	bIsThrown = false;
	bIsLanded = false;
	bShouldMovetoPlayer = false;

	// Turn off SimulatePhysics First, and than Turn Off Collision
	Super::SetInInventory();
}

void AHook::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWaterBody* Water = Cast<AWaterBody>(OtherActor))
	{
		UGameplayStatics::PlaySoundAtLocation( GetWorld() , FallInWaterSound , GetActorLocation() , FRotator::ZeroRotator );
		//LOG( TEXT( "It's a Water!" ) );
		bIsLanded = true;
		BoxComponent->SetSimulatePhysics( false );
		AddActorWorldOffset( FVector( 0 , 0 , -10 ) , true );

		FVector Direction = GetActorLocation() - PlayerCharacter->GetActorLocation();
		Direction.Z = 0;

		SetActorRotation( FRotationMatrix::MakeFromXZ( Direction , PlayerCharacter->GetActorUpVector() ).Rotator() + FRotator(-90, 180, 0));
	}
	else if (AFloatsamBase* Floatsam = Cast<AFloatsamBase>(OtherActor))
	{
		if ( Floatsam->IsGrabbed() == true ) return;
		Floatsam->Grab();
		Floatsam->AttachToActor( this , FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
		FloatsamSet.Add( Floatsam );

		//bShouldMovetoPlayer = true;
	}
}

void AHook::Launch()
{
	check( UseMontage );
	PlayerCharacter->PlayAnimMontage( UseMontage );
	SetActorRotation( FRotationMatrix::MakeFromXZ( PlayerCharacter->GetActorForwardVector(), PlayerCharacter->GetActorUpVector() ).Rotator() + FRotator( -90 , 180 , 0 ) );
	bIsThrown = true;
	BoxComponent->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	BoxComponent->SetSimulatePhysics( true );
	FVector LaunchDirection = PlayerCharacter->GetActorForwardVector() + PlayerCharacter->GetActorUpVector();
	LaunchDirection.Normalize();
	BoxComponent->AddForce( LaunchDirection * ( ForceAmount + ( ForceAmount * ( CurrentChargeTime / MaxChargeTime ) ) ) );

	UGameplayStatics::PlaySoundAtLocation( GetWorld() , ThrowSound , GetActorLocation() , GetActorRotation() );
	
}

void AHook::Use()
{
	Super::Use();

	if ( bIsThrown == false )
	{
		if (PlayerCharacter->GetCharacterMovement()->MovementMode == MOVE_Swimming && PlayerCharacter->IsOverSeaLevel() == false)
		{
			return;
		}
		// 던지기 전
		bIsCharging = true;
	}
	else
	{
		// 던진 후
		if (bIsLanded) bShouldMovetoPlayer = true;
	}
}

void AHook::StopUse()
{
	Super::StopUse();

	LOG( TEXT( "" ) );

	if (bIsThrown == false)
	{
		// 던지기 전
		if ( bIsCharging )
		{
			bIsCharging = false;
			Launch();
			CurrentChargeTime = 0.0f;

			AInGamePlayerController* PC = PlayerCharacter->GetController<AInGamePlayerController>();
			check( PC );
			PC->SetChargePercent( CurrentChargeTime / MaxChargeTime );
		}
	}
	else
	{
		// 던진 후
		bShouldMovetoPlayer = false;
	}
}

void AHook::Cancel()
{
	Super::Cancel();

	SetInInventory();

	// 갈고리 취소 시 걸린 부유물들이 따라오는 현상 수정
	for (AFloatsamBase* Floatsam : FloatsamSet)
	{
		if (Floatsam)
		{
			Floatsam->Release();
			Floatsam->DetachFromActor( FDetachmentTransformRules::KeepWorldTransform );
		}
	}

	FloatsamSet.Empty();

	SetInHand();
}