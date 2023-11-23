// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/CharacterBase.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Kismet/KismetMathLibrary.h>

#include "../Building/BuildingPartsBase.h"
#include "../MarineCraftGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MarineCraft/PlayerController/InGamePlayerController.h"
#include "../Inventory/PlayerInventoryComponent.h"
#include "../Interface/InteractInterface.h"
#include "MarineCraft/Inventory/Tool/ToolBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(SpringArmComponent);
	//SpringArmComponent->SetupAttachment(GetMesh(), TEXT("head"));
	//SpringArmComponent->AttachToComponent( GetMesh() , FAttachmentTransformRules( EAttachmentRule::SnapToTarget , EAttachmentRule::KeepWorld , EAttachmentRule::KeepWorld , false ) , TEXT( "head" ) );
	SpringArmComponent->SetupAttachment( RootComponent );
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	GhostMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GhostMeshComponent"));
	GhostMeshComponent->SetCollisionProfileName(TEXT("GhostMesh"));

	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultIMC)
			{
				InputSystem->AddMappingContext(DefaultIMC, 0);
			}
		}
	}

	GhostMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnGhostMeshBeginOverlap);
	GhostMeshComponent->OnComponentEndOverlap.AddDynamic( this , &ACharacterBase::ACharacterBase::OnGhostMeshEndOverlap);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult OutHit;

	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * TraceDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor( this );

	//DrawDebugLine( GetWorld() , Start , End , FColor::Cyan );
	if ( GetWorld()->LineTraceSingleByChannel( OutHit , Start , End , ECC_Visibility , CollisionQueryParams ) )
	{
		//LOG(TEXT("Hit Actor : %s"), *OutHit.GetActor()->GetName());

		if ( IInteractInterface* InteractInterface = Cast<IInteractInterface>( OutHit.GetActor() ))
		{
			//LOG( TEXT( "Hit Actor : %s Has InteractInterface" ) , *OutHit.GetActor()->GetName() );

			// Todo : TurnOn InteractWidget;
			AInGamePlayerController* PC = GetController<AInGamePlayerController>();
			check( PC );
			PC->UpdateInteractActor( InteractInterface );
			InteractActor = OutHit.GetActor();
		}
		else
		{
			if (InteractActor)
			{
				// Todo : TurnOff InteractWidget;
				AInGamePlayerController* PC = GetController<AInGamePlayerController>();
				check( PC );
				PC->UpdateInteractActor( nullptr );
				InteractActor = nullptr;
			}
			//LOG( TEXT( "Hit Actor : %s Has Not InteractInterface" ) , *OutHit.GetActor()->GetName() );
		}
	}
	else
	{
		if ( InteractActor )
		{
			// Todo : TurnOff InteractWidget;
			AInGamePlayerController* PC = GetController<AInGamePlayerController>();
			check( PC );
			PC->UpdateInteractActor( nullptr );
			InteractActor = nullptr;
		}
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(Input);
	Input->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
	Input->BindAction(InputAction_Look, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
	Input->BindAction(InputAction_Action, ETriggerEvent::Started, this, &ACharacterBase::StartAction );
	Input->BindAction(InputAction_Action, ETriggerEvent::Completed, this, &ACharacterBase::CompleteAction );
	Input->BindAction(InputAction_CancelAction, ETriggerEvent::Started, this, &ACharacterBase::CancelAction );
	Input->BindAction(InputAction_Dive, ETriggerEvent::Triggered, this, &ACharacterBase::Dive );
	Input->BindAction(InputAction_Interact, ETriggerEvent::Started, this, &ACharacterBase::Interact );
	Input->BindAction(InputAction_QuickSlot, ETriggerEvent::Started, this, &ACharacterBase::QuickSlot );
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();
	//LOG(TEXT("VectorValue : %s"), *VectorValue.ToString());
	/*FVector MoveDirection = (GetActorForwardVector() * VectorValue.Y) + (GetActorRightVector() * VectorValue.X);
	MoveDirection.Normalize();
	AddMovementInput(MoveDirection, MoveSpeed);*/

	
	//AddMovementInput( UKismetMathLibrary::GetForwardVector( GetControlRotation() ) , VectorValue.Y * MoveSpeed );
	AddMovementInput( GetActorForwardVector() , VectorValue.Y * MoveSpeed );
	//AddMovementInput( UKismetMathLibrary::GetRightVector( GetControlRotation() ) , VectorValue.X * MoveSpeed );
	AddMovementInput( GetActorRightVector() , VectorValue.X * MoveSpeed );
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();

	//LOG(TEXT("VectorValue : %s"), *VectorValue.ToString());

	AddControllerPitchInput( -VectorValue.Y );
	AddControllerYawInput( VectorValue.X );
}

void ACharacterBase::StartAction(const FInputActionValue& Value)
{
	if ( AToolBase* ToolBase = Cast<AToolBase>(InventoryComponent->GetCurrentItem()) )
	{
		ToolBase->Use();
	}
}

void ACharacterBase::CompleteAction(const FInputActionValue& Value)
{
	if ( AToolBase* ToolBase = Cast<AToolBase>( InventoryComponent->GetCurrentItem() ) )
	{
		ToolBase->StopUse();
	}
}

void ACharacterBase::CancelAction(const FInputActionValue& Value)
{
	if ( AToolBase* ToolBase = Cast<AToolBase>( InventoryComponent->GetCurrentItem() ) )
	{
		ToolBase->Cancel();
	}
}

void ACharacterBase::Dive(const FInputActionValue& Value)
{
	LOG( TEXT( "DiveValue : %f" ) , Value.Get<float>() );
	AddMovementInput( GetActorUpVector() , Value.Get<float>() * MoveSpeed );
}

void ACharacterBase::Interact(const FInputActionValue& Value)
{
	if (IInteractInterface* Interface = Cast<IInteractInterface>( InteractActor ))
	{
		//LOG( TEXT( "Interact Actor : %s" ) , *InteractActor->GetName() );
		Interface->Interact( this );
	}
}

void ACharacterBase::QuickSlot(const FInputActionValue& Value)
{
	if ( APlayerController* PlayerController = GetController<APlayerController>() )
	{
		if ( UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer() ) )
		{
			FKey PressedKey;
			TArray<FKey> KeyArray = InputSystem->QueryKeysMappedToAction(InputAction_QuickSlot);
			for (FKey Key : KeyArray)
			{
				if (PlayerController->IsInputKeyDown(Key))
				{
					PressedKey = Key;
					break;
				}
			}

			GhostMeshOverlappedActorSet.Empty();
			GhostMeshComponent->SetVisibility( false );

			if (PressedKey.GetFName() == TEXT("One"))
			{
				InventoryComponent->SetCurrentItem(0);
			}
			else if ( PressedKey.GetFName() == TEXT( "Two" ) )
			{
				InventoryComponent->SetCurrentItem(1);
			}
			else if ( PressedKey.GetFName() == TEXT( "Three" ) )
			{
				InventoryComponent->SetCurrentItem(2);
			}
			else if ( PressedKey.GetFName() == TEXT( "Four" ) )
			{
				InventoryComponent->SetCurrentItem(3);
			}
			else if ( PressedKey.GetFName() == TEXT( "Five" ) )
			{
				InventoryComponent->SetCurrentItem(4);
			}
			else if ( PressedKey.GetFName() == TEXT( "Six" ) )
			{
				InventoryComponent->SetCurrentItem(5);
			}
			else if ( PressedKey.GetFName() == TEXT( "Seven" ) )
			{
				InventoryComponent->SetCurrentItem(6);
			}
			else if ( PressedKey.GetFName() == TEXT( "Eight" ) )
			{
				InventoryComponent->SetCurrentItem(7);
			}
			else if ( PressedKey.GetFName() == TEXT( "Nine" ) )
			{
				InventoryComponent->SetCurrentItem(8);
			}
			else if ( PressedKey.GetFName() == TEXT( "Zero" ) )
			{
				InventoryComponent->SetCurrentItem(9);
			}
		}
	}
}

void ACharacterBase::UpdateInventoryWidget()
{
	GetController<AInGamePlayerController>()->UpdateInventoryWidget(InventoryComponent);
}

UCameraComponent* ACharacterBase::GetCameraComponent() const
{
	return CameraComponent;
}

UStaticMeshComponent* ACharacterBase::GetGhostMeshComponent() const
{
	return GhostMeshComponent;
}

TSet<AActor*>& ACharacterBase::GetGhostMeshOverlappedActorSet()
{
	return GhostMeshOverlappedActorSet;
}

void ACharacterBase::SetGhostMeshMaterail()
{
	bool bIsBuildable = GetGhostMeshOverlappedActorSet().Num() == 0;

	int MaterialNum = GhostMeshComponent->GetMaterials().Num();
	for ( int i = 0; i < MaterialNum; ++i )
	{
		if ( bIsBuildable )
		{
			GhostMeshComponent->SetMaterial( i , CanBuildMaterial );
		}
		else
		{
			GhostMeshComponent->SetMaterial( i , CannotBuildMaterial );
		}
	}
}

void ACharacterBase::OnGhostMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GhostMeshOverlappedActorSet.Add( OtherActor );

	SetGhostMeshMaterail();
}

void ACharacterBase::OnGhostMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GhostMeshOverlappedActorSet.Contains( OtherActor ))
	{
		GhostMeshOverlappedActorSet.Remove(GhostMeshOverlappedActorSet.FindId(OtherActor));
	}

	SetGhostMeshMaterail();
}