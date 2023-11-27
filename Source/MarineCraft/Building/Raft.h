// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Raft.generated.h"


class AFoundation;
UCLASS()
class MARINECRAFT_API ARaft : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaft();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TSubclassOf<AFoundation> FoundationClass;
	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess ) )
	float FoundationSize;

	AFoundation* RootFoundation;
	TArray<AFoundation*> OutLineFoundationArray;
};