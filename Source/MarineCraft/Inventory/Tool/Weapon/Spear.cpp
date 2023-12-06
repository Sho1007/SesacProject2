// Fill out your copyright notice in the Description page of Project Settings.


#include "../Weapon/Spear.h"

#include <Kismet/GameplayStatics.h>
#include <Engine/DamageEvents.h>

#include "../../../Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASpear::Use()
{
	Super::Use();

	if ( PlayerCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() ) return;

	UGameplayStatics::PlaySoundAtLocation( GetWorld() , StabSound , GetActorLocation() , GetActorRotation() );

	switch ( PlayerCharacter->GetCharacterMovement()->MovementMode )
	{
	case EMovementMode::MOVE_Walking:
		this->SetActorRelativeLocation( FVector(0, -50, 0) );
		this->SetActorRelativeRotation( FRotator::ZeroRotator );

		PlayerCharacter->PlayAnimMontage( UseMontage );
		break;
	case EMovementMode::MOVE_Swimming:
		PlayerCharacter->PlayAnimMontage( MiddleAttackMontage );
		break;
	}
}

void ASpear::CheckAttackHit()
{
	Super::CheckAttackHit();
	//UE_LOG( LogTemp , Warning , TEXT( "ASpear::CheckAttackHit" ) );

	TArray<FHitResult> OutHitArray;

	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StaticMeshComponent->GetSocketLocation( TEXT( "AttackPoint" ) ), StaticMeshComponent->GetSocketLocation( TEXT( "AttackPoint"		) ), AttackRadius,
		UEngineTypes::ConvertToTraceType( ECC_Visibility ), false, {}, EDrawDebugTrace::None, OutHitArray, true))
	{
		for (FHitResult& OutHit : OutHitArray)
		{
			if ( ACharacter* Character = Cast<ACharacter>(OutHit.GetActor()))
			{
				UE_LOG( LogTemp , Warning , TEXT( "ASpear::CheckAttackHit) Hit Actor : %s" ) , *OutHit.GetActor()->GetName() );
				UE_LOG( LogTemp , Warning , TEXT( "ASpear::CheckAttackHit) Hit Component : %s" ) , *OutHit.Component->GetName() );
				Character->TakeDamage( WeaponDamage , FDamageEvent(UDamageType::StaticClass()) , PlayerCharacter->GetController() , PlayerCharacter);
				CurrentDurability--;

				UGameplayStatics::PlaySoundAtLocation( GetWorld() , AttackSound , OutHit.Location , GetActorRotation() );
				break;
			}
		}
	}

	if ( CurrentDurability == 0.0f )
	{
		// Todo : Destroy 순서가 여기가 맞을까?
		PlayerCharacter->SetQuickSlotItemNull( InventoryIndex );
		Destroy();
	}

	PlayerCharacter->UpdateInventoryWidget();
}

void ASpear::EndAttack()
{
	Super::EndAttack();

	//UE_LOG( LogTemp , Warning , TEXT( "ASpear::EndAttack" ) );
	this->SetActorRelativeLocation( FVector( 0 , -7 , 21 ) );
	this->SetActorRelativeRotation( FRotator( 0 , 0 , -90 ) );
}

void ASpear::SetInHand()
{
	Super::SetInHand();

	StaticMeshComponent->SetVisibility( true );

	FString Log;

	if ( StaticMeshComponent->GetVisibleFlag() )
	{
		Log = TEXT( "Visible" );
	}
	else
	{
		Log = TEXT( "Hide" );
	}

	PlayerCharacter->MyPrintLog(Log);

	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "ToolSocket" ) );
	this->SetActorRelativeLocation( FVector(0, -7, 21) );
	this->SetActorRelativeRotation( FRotator(0, 0, -90) );
}
