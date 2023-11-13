// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class MARINECRAFT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsGrabbed() const;

private:
	void Floating(float DeltaTime);

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;


	// Floating
	bool	bIsFloatingUp;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float	FloatingHeight = 100.0f;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float	FloatingSpeed = 100.0f;
	float	FloatingRate;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	FVector FloatingDirection;

	UPROPERTY(EditInstanceOnly)
	bool bIsGrabbed;
};