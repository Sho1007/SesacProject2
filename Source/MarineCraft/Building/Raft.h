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

	void AddCorner( AFoundation* NewCorner );
	void RemoveCorner( AFoundation* OldCorner );

	UFUNCTION(BlueprintCallable)
	float GetRaftSize() const;

	UFUNCTION(CallInEditor)
	void PrintRaftSize();

	UFUNCTION(BlueprintCallable)
	FVector GetRootLocation() const;

	UFUNCTION(BlueprintCallable)
	FVector GetMaxDistance() const;

	UFUNCTION( BlueprintCallable )
	TArray<AFoundation*> GetCornerArray() const;
private:
	void SetRootFoundation();
	void RemoveSeparatedFoundation();
private:
	// Spawn Raft Setting
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TSubclassOf<AFoundation> FoundationClass;
	UPROPERTY( EditAnywhere , Meta = ( AllowPrivateAccess ) )
	float FoundationSize;

	// Raft Graph
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	AFoundation* RootFoundation;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	TArray<AFoundation*> FoundationArray;
	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	FVector RootLocation = FVector::ZeroVector;

	UPROPERTY( VisibleInstanceOnly , Meta = ( AllowPrivateAccess ) )
	TSet<AFoundation*> CornerSet;


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