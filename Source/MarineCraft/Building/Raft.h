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

	void AddFoundtaion(AFoundation* NewFoundation);
	void RemoveFoundation(AFoundation* NewFoundation);

	UFUNCTION(BlueprintCallable)
	FVector GetRootLocation() const;

private:
	// Spawn Raft Setting
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TSubclassOf<AFoundation> FoundationClass;
	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess ) )
	float FoundationSize;

	// Raft Graph
	AFoundation* RootFoundation;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	TMap<FString, AFoundation*> FoundationMap;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	FVector RootLocation = FVector::ZeroVector;

	// Move
	UPROPERTY(EditAnywhere , Category = "Floating" , Meta = (AllowPrivateAccess))
	FVector MoveDirection;
	UPROPERTY( EditAnywhere , Category = "Floating" , Meta = ( AllowPrivateAccess ) )
	float MoveSpeed;


	// Floating
	bool bFloatingUp;
	UPROPERTY( EditAnywhere , Category = "Floating" , Meta = ( AllowPrivateAccess ) )
	float FloatingHeight;
	UPROPERTY( EditAnywhere , Category = "Floating", Meta = ( AllowPrivateAccess ) )
	float BaseHeight;
	UPROPERTY( EditAnywhere , Category = "Floating", Meta = ( AllowPrivateAccess ) )
	float FloatingSpeed;
};