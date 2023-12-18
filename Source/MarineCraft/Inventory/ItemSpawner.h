// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

class ARaft;
class AFloatsamBase;
class ACharacterBase;
UCLASS()
class MARINECRAFT_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnItem();

private:
	// Player
	UPROPERTY( VisibleInstanceOnly, Meta = ( AllowPrivateAccess ) )
	ACharacterBase* Player;
	// Raft
	UPROPERTY( VisibleInstanceOnly, Meta = ( AllowPrivateAccess ) )
	ARaft* Raft;
	// SpawnTime
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Item", Meta = (AllowPrivateAccess))
	float ItemSpawnTime;
	float CurrentItemSpawnTime;
	// Spawn Class
	UPROPERTY( EditDefaultsOnly , Category = "Spawn Item" , Meta = ( AllowPrivateAccess ) )
	TArray<TSubclassOf<AFloatsamBase>> FloatsamClassArray;

	UPROPERTY( EditAnywhere , Category = "Spawn Item" , Meta = ( AllowPrivateAccess ) )
	FVector OceanCurrentsDirection;
	// 스폰 거리
	UPROPERTY( EditAnywhere , Category = "Spawn Item" , Meta = ( AllowPrivateAccess ) )
	float SpawnDistance;
	// 스폰 오프셋 (좌우로 얼마만큼의 간격을 랜덤하게 띄울지의 절반 거리)
	UPROPERTY( EditAnywhere , Category = "Spawn Item" , Meta = ( AllowPrivateAccess ) )
	float SpawnOffset;
};