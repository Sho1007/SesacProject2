// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "GameFramework/Actor.h"

#include <Engine/DataTable.h>

#include "ItemBase.generated.h"

class AItemBase;
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	FName ItemName;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	int32 MaxStack;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	UTexture2D* ItemImage;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	TSubclassOf<AItemBase> ItemClass;

	FItemData() : ItemName(TEXT("")), MaxStack(1), ItemImage(nullptr)
	{
		
	}
};

USTRUCT( BlueprintType )
struct FItemInstanceData
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	FName ItemName;
	UPROPERTY( BlueprintReadWrite , EditAnywhere )
	int32 CurrentStack;

	FItemInstanceData() : ItemName(TEXT("")), CurrentStack(1)
	{
		
	}
};

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

	// Getter
	const FItemInstanceData* GetInstanceData() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
	FItemInstanceData InstanceData;
};