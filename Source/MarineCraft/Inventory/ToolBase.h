// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolBase.generated.h"

class UBoxComponent;
UCLASS()
class MARINECRAFT_API AToolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToolBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY( EditDefaultsOnly)
	UBoxComponent* BoxComponent;
	UPROPERTY( EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;
};
