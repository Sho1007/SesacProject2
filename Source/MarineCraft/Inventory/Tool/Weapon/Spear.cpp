// Fill out your copyright notice in the Description page of Project Settings.


#include "../Weapon/Spear.h"

#include "../../../Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASpear::Use()
{
	Super::Use();

	switch ( PlayerCharacter->GetCharacterMovement()->MovementMode )
	{
	case EMovementMode::MOVE_Walking:
		PlayerCharacter->PlayAnimMontage( UseMontage );
		break;
	case EMovementMode::MOVE_Swimming:
		break;
	}
}

void ASpear::SetInHand()
{
	Super::SetInHand();

	StaticMeshComponent->SetVisibility( true );
	this->AttachToComponent( PlayerCharacter->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , TEXT( "ToolSocket" ) );
	this->SetActorRelativeLocation( FVector(0, -7, 21) );
	this->SetActorRelativeRotation( FRotator(0, 0, -90) );
}
