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

// Sets default values
ACharacterBase::ACharacterBase() : bIsBuildMode(true)
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
	GhostMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

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
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBuildMode)
	{
		FHitResult OutHit;

		FVector Start = CameraComponent->GetComponentLocation();
		FVector End = Start + CameraComponent->GetForwardVector() * TraceDistance;

		FCollisionObjectQueryParams Params;
		Params.AddObjectTypesToQuery(ECC_GameTraceChannel1);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionQueryParams))
		{
			//LOG(TEXT("Hit Actor : %s"), *OutHit.GetActor()->GetName());
			if (BuildTargetComponent == OutHit.GetComponent()) return;

			LOG(TEXT("ComponentName : %s"), *OutHit.GetComponent()->GetName());

			if (ABuildingPartsBase* BuildParts = Cast<ABuildingPartsBase>(OutHit.GetComponent()->GetOwner()))
			{
				FName BuildingPartsName = BuildParts->GetBuildingPartsName(OutHit.GetComponent());
				if (BuildingPartsName.Compare(TEXT("None")) != 0)
				{
					BuildTargetComponent = OutHit.GetComponent();

					UMarineCraftGameInstance* GameInstance = GetGameInstance<UMarineCraftGameInstance>();
					check(GameInstance);
					FBuildingPartsData* BuildingPartsData = GameInstance->GetBuildingPartsData(BuildingPartsName);
					check(BuildingPartsData);

					GhostMeshComponent->SetStaticMesh(BuildingPartsData->Mesh);
					GhostMeshComponent->SetWorldRotation(BuildTargetComponent->GetComponentRotation());
					GhostMeshComponent->SetWorldLocation(BuildTargetComponent->GetComponentLocation() + BuildTargetComponent->GetComponentRotation().RotateVector(BuildingPartsData->MeshLocationOffset));
					GhostMeshComponent->SetWorldScale3D(BuildingPartsData->Scale);

					int MaterialNum = GhostMeshComponent->GetMaterials().Num();
					for (int i = 0; i < MaterialNum; ++i)
					{
						GhostMeshComponent->SetMaterial(i, CanBuildMaterial);
					}
					GhostMeshComponent->SetVisibility(true);

					return;
				}
			}
		}
	}

	BuildTargetComponent = nullptr;
	GhostMeshComponent->SetVisibility(false);

	if (bIsCharging)
	{
		Charge(DeltaTime);
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
	Input->BindAction(InputAction_Dive, ETriggerEvent::Triggered, this, &ACharacterBase::Dive );
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
	LOG( TEXT( "" ) );
	if (bIsBuildMode && BuildTargetComponent != nullptr)
	{
		// Spawn BuildingPartsActor
		if (ABuildingPartsBase* BuildParts = Cast<ABuildingPartsBase>(BuildTargetComponent->GetOwner()))
		{
			PlayAnimMontage(AttackMontage);
			FName BuildingPartsName = BuildParts->GetBuildingPartsName(BuildTargetComponent);

			UMarineCraftGameInstance* GameInstance = GetGameInstance<UMarineCraftGameInstance>();
			check(GameInstance);
			FBuildingPartsData* BuildingPartsData = GameInstance->GetBuildingPartsData(BuildingPartsName);
			check(BuildingPartsData);

			ABuildingPartsBase* NewBuildingParts = GetWorld()->SpawnActor<ABuildingPartsBase>(BuildingPartsData->Class, BuildTargetComponent->GetComponentLocation() + BuildingPartsData->SpawnLocationOffset, BuildTargetComponent->GetComponentRotation());
			BuildTargetComponent = nullptr;
			GhostMeshComponent->SetVisibility(false);
		}
	}
	// Todo : 내 퀵슬롯의 현재 아이템이 [Charge 가능한] [도구] 라면 bIsCharging을 True 로 한다.
	// Charge Test
	bIsCharging = true;
}

void ACharacterBase::CompleteAction(const FInputActionValue& Value)
{
	LOG( TEXT( "" ) );

	// Charge Test
	if ( bIsCharging ) Uncharge();
}

void ACharacterBase::Dive(const FInputActionValue& Value)
{
	LOG( TEXT( "DiveValue : %f" ) , Value.Get<float>() );
	AddMovementInput( GetActorUpVector() , Value.Get<float>() * MoveSpeed );
}

void ACharacterBase::Charge(float DeltaTime)
{
	ChargeValue += DeltaTime;

	LOG( TEXT( "Charge Value : %f" ) , ChargeValue );

	if (AInGamePlayerController* PC = GetController<AInGamePlayerController>())
	{
		// Todo : Change 1.0f to Tool's MaxChargeTime
		PC->SetChargePercent(ChargeValue / 1.0f);
	}
}

void ACharacterBase::Uncharge()
{
	ChargeValue = 0.0f;
	bIsCharging = false;
	if ( AInGamePlayerController* PC = GetController<AInGamePlayerController>() )
	{
		PC->SetChargePercent( 0.0f );
	}
}