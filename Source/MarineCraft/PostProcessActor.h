// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PostProcessActor.generated.h"

class UPostProcessComponent;

UCLASS()
class MARINECRAFT_API APostProcessActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APostProcessActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMaterialWeight(float NewMaterialWeight);

private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	float OilMaterialWeight = 0.5f;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float RemoveOilSpeed = 0.01f;
	
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UPostProcessComponent* PostProcessComponent;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UMaterialInstance* OilMaterial;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* DynamicMaterial; 
};