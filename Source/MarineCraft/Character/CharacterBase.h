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

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
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

	UPROPERTY(EditDefaultsOnly, Category = "Component", Meta = (AllowPrivateAccess))
	UCameraComponent* CameraComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Component", Meta = (AllowPrivateAccess))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Move", Meta = (AllowPrivateAccess))
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputMappingContext* DefaultIMC;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Look;
};