// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UStatusComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class USpringArmComponent;
struct FBuildingPartsData;
class UPlayerInventoryComponent;
class ARaft;
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

	virtual float TakeDamage( float DamageAmount , struct FDamageEvent const& DamageEvent , class AController* EventInstigator , AActor* DamageCauser ) override;

	UFUNCTION(BlueprintCallable)
	void ShowUI();
	UFUNCTION(BlueprintCallable)
	void HideUI();

	// InputAction
	void DoJump( const FInputActionValue& Value );
	void Move(const FInputActionValue& Value);
	void EndMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartAction(const FInputActionValue& Value);
	void CompleteAction( const FInputActionValue& Value );
	void CancelAction( const FInputActionValue& Value );
	void Dive( const FInputActionValue& Value );
	void Interact( const FInputActionValue& Value );
	void QuickSlot( const FInputActionValue& Value );
	void ToggleInventory( const FInputActionValue& Value );

	// Attack
	UFUNCTION(BlueprintCallable)
	void EndAttack();
	UFUNCTION(BlueprintCallable)
	void CheckAttackHit();

	// Tool
	UFUNCTION( BlueprintCallable )
	void StopUse();

	UFUNCTION( BlueprintImplementableEvent )
	void MyPrintLog(const FString& message );

	// Building
	void UpdateInventoryWidget();
	UCameraComponent* GetCameraComponent() const;
	UStaticMeshComponent* GetGhostMeshComponent() const;
	TSet<AActor*>& GetGhostMeshOverlappedActorSet();
	void SetGhostMeshMaterial();
	UFUNCTION()
	void OnGhostMeshBeginOverlap( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );
	UFUNCTION()
	void OnGhostMeshEndOverlap( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex );

	ARaft* GetRaft() const;

	// Inventory
	void SetQuickSlotItemNull( int32 QuickSlotIndex );

	// State
	void Die();
	bool IsOverSeaLevel() const;
	UFUNCTION(BlueprintCallable)
	bool IsSwim() const;
	UFUNCTION( BlueprintCallable )
	bool IsOnRaft();
	UFUNCTION( BlueprintCallable )
	bool IsDead();

	// Look At
	AActor* GetLookingAtActor() const;

private:
	void StartSwim();
	void EndSwim();

private:
	// Action?
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = (AllowPrivateAccess))
	UAnimMontage* AttackMontage;

	// Check On Raft
	UPROPERTY( EditDefaultsOnly, Meta = ( AllowPrivateAccess ) )
	float CheckRaftDistance;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bIsOnRaft;

	// Build
	ARaft* Raft;
	UPROPERTY(EditDefaultsOnly, Category = "Build", Meta = (AllowPrivateAccess))
	UStaticMeshComponent* GhostMeshComponent;
	UPROPERTY( VisibleAnywhere, Category = "Build" , Meta = ( AllowPrivateAccess ) )
	TSet<AActor*> GhostMeshOverlappedActorSet;
	UPROPERTY( EditDefaultsOnly , Category = "Build" , Meta = ( AllowPrivateAccess ) )
	UMaterialInstance* CanBuildMaterial;
	UPROPERTY( EditDefaultsOnly , Category = "Build" , Meta = ( AllowPrivateAccess ) )
	UMaterialInstance* CannotBuildMaterial;

	// Trace
	UPROPERTY(EditDefaultsOnly, Category = "Interact", Meta = (AllowPrivateAccess))
	float TraceDistance;

	// Trace - What Player currently look at
	AActor* LookingAtActor;

	// Trace - Interact
	AActor* InteractActor;

	// Move
	UPROPERTY( EditDefaultsOnly , Category = "Move" , Meta = ( AllowPrivateAccess ) )
	float MoveSpeed;
	// Move - Swim
	UPROPERTY( BlueprintReadOnly , EditDefaultsOnly , Category = "Swim" , Meta = ( AllowPrivateAccess ) )
	float SwimmingHeight;
	UPROPERTY( EditDefaultsOnly , Category = "Move" , Meta = ( AllowPrivateAccess ) )
	float DiveValue;

	// Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component", Meta = (AllowPrivateAccess))
	UCameraComponent* CameraComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Component", Meta = (AllowPrivateAccess))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (AllowPrivateAccess))
	UPlayerInventoryComponent* InventoryComponent;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess))
	UStatusComponent* StatusComponent;

private:
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
	UInputAction* InputAction_CancelAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Dive;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Interact;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_QuickSlot;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_ToggleInventory;
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (AllowPrivateAccess))
	UInputAction* InputAction_Jump;

	// Sound
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* InteractSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* BGMSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* OceanSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* ScreamSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* DeathSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* StartSwimSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* SwimmingSound;
	UPROPERTY( EditDefaultsOnly , Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	USoundBase* EndSwimSound;

	// Sound Component
	UPROPERTY( VisibleInstanceOnly, Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	UAudioComponent* OceanSoundComponent;
	UPROPERTY( VisibleInstanceOnly, Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	UAudioComponent* SwimmingSoundComponent;
	UPROPERTY( VisibleInstanceOnly, Category = "SFX" , Meta = ( AllowPrivateAccess ) )
	UAudioComponent* BGMSoundComponent;
};