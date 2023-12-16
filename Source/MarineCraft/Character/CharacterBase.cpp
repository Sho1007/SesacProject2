// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/CharacterBase.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>

#include "../Building/BuildingPartsBase.h"
#include "../MarineCraftGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MarineCraft/PlayerController/InGamePlayerController.h"
#include "../Inventory/PlayerInventoryComponent.h"
#include "../Interface/InteractInterface.h"
#include "MarineCraft/Inventory/Tool/BuildingHammer.h"
#include "MarineCraft/Inventory/Tool/ToolBase.h"
#include "../Building/Raft.h"
#include "GameFramework/PhysicsVolume.h"
#include "MarineCraft/Inventory/Tool/Weapon/WeaponBase.h"
#include "StatusComponent.h"
#include "Components/AudioComponent.h"
#include "MarineCraft/Building/Foundation.h"
#include "MarineCraft/Inventory/Placeable/PlaceableBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(SpringArmComponent);
	SpringArmComponent->SetupAttachment(GetMesh(), TEXT("head"));
	//SpringArmComponent->AttachToComponent( GetMesh() , FAttachmentTransformRules( EAttachmentRule::SnapToTarget , EAttachmentRule::KeepWorld , EAttachmentRule::KeepWorld , false ) , TEXT( "head" ) );
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	GhostMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GhostMeshComponent"));
	GhostMeshComponent->SetCollisionProfileName(TEXT("GhostMesh"));

	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));

	StatusComponent = CreateDefaultSubobject<UStatusComponent>( TEXT( "StatusComponent" ) );
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// SpringArmComponent->AttachToComponent( GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "Head" ) );
	
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

	// Get Raft Actor
	TActorIterator<AActor> It( GetWorld() , ARaft::StaticClass() );
	Raft = Cast<ARaft>( *It );
	check( Raft );

	OceanSoundComponent = UGameplayStatics::SpawnSoundAttached( OceanSound , GetRootComponent() );
	BGMSoundComponent = UGameplayStatics::SpawnSound2D( GetWorld() , BGMSound );
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GhostMeshComponent->GetVisibleFlag() == true) MyPrintLog(TEXT("Visible"));
	else MyPrintLog( TEXT( "Hidden" ) );
	

	FHitResult OutHit;

	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * TraceDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor( this );

	//DrawDebugLine( GetWorld() , Start , End , FColor::Cyan );
	if ( GetWorld()->LineTraceSingleByChannel( OutHit , Start , End , ECC_Visibility , CollisionQueryParams ) )
	{
		LookingAtActor = OutHit.GetActor();

		//UE_LOG( LogTemp , Warning , TEXT( "ACharacterBase::Tick) LookingAtActor : %s" ), *LookingAtActor->GetActorLabel() );

		if ( IInteractInterface* InteractInterface = Cast<IInteractInterface>( OutHit.GetActor() ) )
		{
			// Todo : TurnOn InteractWidget;
			AInGamePlayerController* PC = GetController<AInGamePlayerController>();
			check( PC );
			PC->UpdateInteractActor( InteractInterface );
			InteractActor = OutHit.GetActor();
			// UE_LOG( LogTemp , Warning , TEXT( "ACharacterBase::Tick ) Interact Actor : %s" ) , *InteractActor->GetActorLabel() );
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
		}
	}
	else
	{
		LookingAtActor = nullptr;

		if ( InteractActor )
		{
			// Todo : TurnOff InteractWidget;
			AInGamePlayerController* PC = GetController<AInGamePlayerController>();
			check( PC );
			PC->UpdateInteractActor( nullptr );
			InteractActor = nullptr;
		}
	}

	// Swim
	if (GetCharacterMovement()->MovementMode == MOVE_Swimming)
	{
		// Swim	

		if ( GetActorLocation().Z > SwimmingHeight )
		{
			EndSwim();
		}
	}
	else
	{
		// Not Swim (Walk or Jump or etc)

		TArray<FHitResult> OutHitArray;

		if ( GetActorLocation().Z <= SwimmingHeight )
		{
			StartSwim();
		}
		else
		{
			Start = GetActorLocation();
			End = Start + GetActorUpVector() * -CheckRaftDistance;
			if ( GetWorld()->LineTraceMultiByChannel( OutHitArray , Start , End , ECC_Visibility , CollisionQueryParams ) )
			{
				bool bFoundRaft = false;
				for (FHitResult& CurrentOutHit : OutHitArray )
				{
					if (Cast<ABuildingPartsBase>(CurrentOutHit.GetActor()))
					{
						bFoundRaft = true;
						break;
					}
				}

				if ( bFoundRaft )
				{
					bIsOnRaft = true;
				}
			}
			else
			{
				bIsOnRaft = false;
			}
		}
	}

	if (bIsOnRaft) MyPrintLog( TEXT( "IsOnRaft" ) );
	else MyPrintLog( TEXT( "IsNotOnRaft" ) );
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(Input);
	Input->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
	Input->BindAction(InputAction_Move, ETriggerEvent::Completed, this, &ACharacterBase::EndMove);
	Input->BindAction(InputAction_Look, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
	Input->BindAction(InputAction_Action, ETriggerEvent::Started, this, &ACharacterBase::StartAction );
	Input->BindAction(InputAction_Action, ETriggerEvent::Completed, this, &ACharacterBase::CompleteAction );
	Input->BindAction(InputAction_CancelAction, ETriggerEvent::Started, this, &ACharacterBase::CancelAction );
	Input->BindAction(InputAction_Dive, ETriggerEvent::Triggered, this, &ACharacterBase::Dive );
	Input->BindAction(InputAction_Interact, ETriggerEvent::Started, this, &ACharacterBase::Interact );
	Input->BindAction(InputAction_QuickSlot, ETriggerEvent::Started, this, &ACharacterBase::QuickSlot );
	Input->BindAction(InputAction_ToggleInventory, ETriggerEvent::Started, this, &ACharacterBase::ToggleInventory );
	Input->BindAction(InputAction_Jump, ETriggerEvent::Started, this, &ACharacterBase::DoJump );
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	// LOG( TEXT( "DamageCauser : %s" ) , *DamageCauser->GetName() );
	this;
	if ( StatusComponent->IsDead() == false )
	{
		StatusComponent->AddDamage( DamageAmount );

		if ( StatusComponent->IsDead() == false  )
		{
			UGameplayStatics::PlaySoundAtLocation( GetWorld() , ScreamSound , GetActorLocation() , GetActorRotation() );
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation( GetWorld() , DeathSound , GetActorLocation() , GetActorRotation() );
		}
	}

	return Super::TakeDamage(DamageAmount , DamageEvent , EventInstigator , DamageCauser);
}

void ACharacterBase::DoJump(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->MovementMode == MOVE_Swimming)
	{
		if ( GetActorLocation().Z >= SwimmingHeight )
		{
			GetCharacterMovement()->JumpZVelocity = 550.0f;
			EndSwim();
			Jump();
		}
	}
	else
	{
		GetCharacterMovement()->JumpZVelocity = 420.0f;
		Jump();
	}
	
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();
	//LOG(TEXT("VectorValue : %s"), *VectorValue.ToString());
	/*FVector MoveDirection = (GetActorForwardVector() * VectorValue.Y) + (GetActorRightVector() * VectorValue.X);
	MoveDirection.Normalize();
	AddMovementInput(MoveDirection, MoveSpeed);*/

	if ( GetCharacterMovement()->MovementMode == MOVE_Swimming )
	{
		if ( SwimmingSoundComponent )
		{
			if ( SwimmingSoundComponent->IsPlaying() == false )
			{
				SwimmingSoundComponent->Play();
			}
		}
		else
		{
			SwimmingSoundComponent = UGameplayStatics::SpawnSoundAttached( SwimmingSound , GetRootComponent() );
		}
	}
	
	//AddMovementInput( UKismetMathLibrary::GetForwardVector( GetControlRotation() ) , VectorValue.Y * MoveSpeed );
	AddMovementInput( GetActorForwardVector() , VectorValue.Y * MoveSpeed );
	//AddMovementInput( UKismetMathLibrary::GetRightVector( GetControlRotation() ) , VectorValue.X * MoveSpeed );
	AddMovementInput( GetActorRightVector() , VectorValue.X * MoveSpeed );
}

void ACharacterBase::EndMove(const FInputActionValue& Value)
{
	if ( SwimmingSoundComponent && SwimmingSoundComponent->IsPlaying() ) SwimmingSoundComponent->Stop();
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
	else if ( APlaceableBase* Placeable = Cast<APlaceableBase>(InventoryComponent->GetCurrentItem()) )
	{
		Placeable->Place();
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
	//LOG( TEXT( "DiveValue : %f" ) , Value.Get<float>() );
	if ( GetCharacterMovement()->MovementMode == MOVE_Swimming )
	{
		if (GetActorLocation().Z + Value.Get<float>() * MoveSpeed * GetWorld()->GetDeltaSeconds() >= SwimmingHeight )
		{
			FVector Destination = GetActorLocation();
			Destination.Z = SwimmingHeight;
			SetActorLocation( Destination );
			GetCharacterMovement()->Velocity.Z = 0.0f;
		}
		else
		{
			AddMovementInput( GetActorUpVector() , Value.Get<float>() * MoveSpeed );
		}
	}
}

void ACharacterBase::Interact(const FInputActionValue& Value)
{
	if (IInteractInterface* Interface = Cast<IInteractInterface>( InteractActor ))
	{
		//LOG( TEXT( "Interact Actor : %s" ) , *InteractActor->GetName() );

		UGameplayStatics::PlaySoundAtLocation( GetWorld() , InteractSound , InteractActor->GetActorLocation() );

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

			StopAnimMontage();

			GhostMeshOverlappedActorSet.Empty();
			GhostMeshComponent->SetVisibility( false );
			GhostMeshComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );

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

void ACharacterBase::ToggleInventory(const FInputActionValue& Value)
{
	UE_LOG( LogTemp , Warning , TEXT( "ACharacterBase::ToggleInventory" ) );
	GetController<AInGamePlayerController>()->ToggleInventory();
}

void ACharacterBase::EndAttack()
{
	if ( AWeaponBase* Weapon = Cast<AWeaponBase>( InventoryComponent->GetCurrentItem() ) )
	{
		Weapon->EndAttack();
	}
}

void ACharacterBase::CheckAttackHit()
{
	if ( AWeaponBase* Weapon = Cast<AWeaponBase>( InventoryComponent->GetCurrentItem() ) )
	{
		Weapon->CheckAttackHit();
	}
}

void ACharacterBase::StopUse()
{
	if ( AToolBase* Tool = Cast<AToolBase>( InventoryComponent->GetCurrentItem() ) )
	{
		Tool->StopUse();
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

ARaft* ACharacterBase::GetRaft() const
{
	return Raft;
}

void ACharacterBase::SetGhostMeshMaterial()
{
	bool bIsBuildable;

	if ( ABuildingHammer* BuildingHammer = Cast<ABuildingHammer>( InventoryComponent->GetCurrentItem() ))
	{
		// 손에 들고 있는 것이 건설 망치라면 건축물의 재료를 체크
		FBuildingPartsData* BuildingPartsData = BuildingHammer->GetBuildingPartsData();
		if ( BuildingPartsData == nullptr ) return;
		bIsBuildable = ( GetGhostMeshOverlappedActorSet().Num() == 0 ) && InventoryComponent->CanRemovableItems( BuildingPartsData->BuildingMaterialMap );
	}
	else
	{
		// 아니라면 단순히 충돌체가 없는지만 체크
		bIsBuildable = GetGhostMeshOverlappedActorSet().Num() == 0;
	}

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

void ACharacterBase::SetQuickSlotItemNull(int32 QuickSlotIndex)
{
	InventoryComponent->SetQuickSlotItemNull( QuickSlotIndex );
	UpdateInventoryWidget();
}

void ACharacterBase::OnGhostMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( Cast<AFoundation>( OtherActor ) ) return;

	//UE_LOG( LogTemp , Warning , TEXT( "ACharacterBase::OnGhostMeshBeginOverlap) Overlapped Actor : %s" ), *OtherActor->GetActorLabel());

	GhostMeshOverlappedActorSet.Add( OtherActor );

	SetGhostMeshMaterial();
}

void ACharacterBase::OnGhostMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GhostMeshOverlappedActorSet.Contains( OtherActor ))
	{
		GhostMeshOverlappedActorSet.Remove(GhostMeshOverlappedActorSet.FindId(OtherActor));
	}

	SetGhostMeshMaterial();
}

void ACharacterBase::Die()
{
	GetController<AInGamePlayerController>()->Die();

	GetMesh()->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	GetMesh()->SetSimulatePhysics( true );
}

bool ACharacterBase::IsOverSeaLevel() const
{
	return GetActorLocation().Z >= SwimmingHeight;
}

bool ACharacterBase::IsSwim() const
{
	return GetCharacterMovement()->MovementMode == MOVE_Swimming;
}

bool ACharacterBase::IsOnRaft()
{
	return bIsOnRaft;
}

bool ACharacterBase::IsDead()
{
	return StatusComponent->IsDead();
}

AActor* ACharacterBase::GetLookingAtActor() const
{
	return LookingAtActor;
}

void ACharacterBase::StartSwim()
{
	UE_LOG( LogTemp , Warning , TEXT( "Start Swim" ) );
	UGameplayStatics::PlaySoundAtLocation( GetWorld() , StartSwimSound , GetActorLocation() , FRotator::ZeroRotator );
	GetCharacterMovement()->SetMovementMode( MOVE_Swimming );
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
}
void ACharacterBase::EndSwim()
{
	UE_LOG( LogTemp , Warning , TEXT( "End Swim" ) );
	UGameplayStatics::PlaySoundAtLocation( GetWorld() , EndSwimSound , GetActorLocation() , FRotator::ZeroRotator );
	GetCharacterMovement()->SetMovementMode( MOVE_Walking );
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
}