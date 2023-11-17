﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatsamBase.generated.h"

class UBoxComponent;
UCLASS()
class MARINECRAFT_API AFloatsamBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatsamBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getter / Setter
	bool IsGrabbed() const;
	void Grab();
private:
	void Floating( float DeltaTime );

protected:
	UPROPERTY( EditDefaultsOnly )
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY( EditDefaultsOnly )
	UBoxComponent* BoxComponent;

	// Floating
	bool	bIsFloatingUp;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float	FloatingHeight = 5.0f;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	float	FloatingSpeed = 0.8f;
	float	FloatingRate;
	UPROPERTY( EditDefaultsOnly , Meta = ( AllowPrivateAccess ) )
	FVector FloatingDirection;

	UPROPERTY( EditInstanceOnly )
	bool bIsGrabbed;
};