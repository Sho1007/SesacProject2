// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "ToolBase.h"
#include "Hook.generated.h"

class UStaticMeshComponent;
class UCableComponent;
class AFloatsamBase;
class ACharacterBase;
UCLASS()
class MARINECRAFT_API AHook : public AToolBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );

	UFUNCTION(CallInEditor)
	void Launch();

	virtual void Use() override;
	virtual void StopUse() override;
	virtual void Cancel() override;

private:
	void MoveToPlayer( float DeltaTime );
	void Catch();

protected:
	virtual void SetInHand() override;
	virtual void SetInInventory() override;

private:
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	TSet<AFloatsamBase*> FloatsamSet;
	UPROPERTY( EditDefaultsOnly, Meta = ( AllowPrivateAccess ) )
	float ForceAmount;
	UPROPERTY( EditDefaultsOnly, Meta = ( AllowPrivateAccess ) )
	float PullSpeed;
	UPROPERTY( EditDefaultsOnly, Meta = ( AllowPrivateAccess ) )
	float CatchDistance;
	
	UPROPERTY( VisibleInstanceOnly, Meta = ( AllowPrivateAccess ) )
	ACharacterBase* PlayerCharacter;

	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bIsThrown;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bIsLanded;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	bool bShouldMovetoPlayer;


	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	UCableComponent* CableComponent;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	UStaticMeshComponent* RopeMeshComponent;
};