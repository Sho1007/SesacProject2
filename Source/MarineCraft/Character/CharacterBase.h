// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class USpringArmComponent;
struct FBuildingPartsData;
class UPlayerInventoryComponent;
UCLASS()
class MARINECRAFT_API ACharacterBase : public ACharacter
{	
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// InputAction
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartAction(const FInputActionValue& Value);
	void CompleteAction( const FInputActionValue& Value );
	void Dive( const FInputActionValue& Value );
	void Interact( const FInputActionValue& Value );
	void QuickSlot( const FInputActionValue& Value );

private:
	void Charge(float DeltaTime);
	void Uncharge();
	// Todo : Rename (Find Build Component?)
	void BuildFunc( FHitResult& OutHit );

private:
	// Interact
	AActor* InteractActor;

	// Charge
	UPROPERTY( VisibleInstanceOnly , Category = "State" , Meta = ( AllowPrivateAccess ) )
	float ChargeValue;
	UPROPERTY( VisibleInstanceOnly, Category = "State" , Meta = ( AllowPrivateAccess ) )
	bool bIsCharging;

	// Action?
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = (AllowPrivateAccess))
	UAnimMontage* AttackMontage;

	// Build
	UPROPERTY(EditDefaultsOnly, Category = "Build", Meta = (AllowPrivateAccess))
	UMaterialInstance* CanBuildMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Build", Meta = (AllowPrivateAccess))
	UMaterialInstance* CannotBuildMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Build", Meta = (AllowPrivateAccess))
	UStaticMeshComponent* GhostMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Build", Meta = (AllowPrivateAccess))
	UPrimitiveComponent* BuildTargetComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Build", Meta = (AllowPrivateAccess))
	bool bIsBuildMode;
	UPROPERTY(EditDefaultsOnly, Category = "Build", Meta = (AllowPrivateAccess))
	float TraceDistance;

	UPROPERTY( EditDefaultsOnly , Category = "Move" , Meta = ( AllowPrivateAccess ) )
	float MoveSpeed;
	UPROPERTY( EditDefaultsOnly , Category = "Move" , Meta = ( AllowPrivateAccess ) )
	float DiveValue;

	// Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component", Meta = (AllowPrivateAccess))
	UCameraComponent* CameraComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Component", Meta = (AllowPrivateAccess))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	UPlayerInventoryComponent* InventoryComponent;
	
	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputMappingContext* DefaultIMC;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Look;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Dive;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Interact;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_QuickSlot;
};