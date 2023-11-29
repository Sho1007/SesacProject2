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
	UE_LOG( LogTemp , Warning , TEXT( "ASpear::CheckAttackHit" ) );

	TArray<FHitResult> OutHitArray;

	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StaticMeshComponent->GetSocketLocation( TEXT( "AttackPoint" ) ), StaticMeshComponent->GetSocketLocation( TEXT( "AttackPoint"		) ), AttackRadius,
		UEngineTypes::ConvertToTraceType( ECC_Visibility ), false, {}, EDrawDebugTrace::ForDuration, OutHitArray, true))
	{
		for (FHitResult& OutHit : OutHitArray)
		{
			if ( ACharacter* Character = Cast<ACharacter>(OutHit.GetActor()))
			{
				UE_LOG( LogTemp , Warning , TEXT( "ASpear::CheckAttackHit) Hit Actor : %s" ) , *OutHit.GetActor()->GetName() );
				Character->TakeDamage( WeaponDamage , FDamageEvent(UDamageType::StaticClass()) , PlayerCharacter->GetController() , PlayerCharacter);
			}
		}
	}
}

void ASpear::EndAttack()
{
	Super::EndAttack();

	UE_LOG( LogTemp , Warning , TEXT( "ASpear::EndAttack" ) );
	this->SetActorRelativeLocation( FVector( 0 , -7 , 21 ) );
	this->SetActorRelativeRotation( FRotator( 0 , 0 , -90 ) );
}

void ASpear::SetInHand()
{
	Super::SetInHand();

	StaticMeshComponent->SetVisibility( true );
	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "ToolSocket" ) );
	this->SetActorRelativeLocation( FVector(0, -7, 21) );
	this->SetActorRelativeRotation( FRotator(0, 0, -90) );
}
