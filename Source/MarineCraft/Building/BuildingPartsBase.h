// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <Engine/DataTable.h>

#include "BuildingPartsBase.generated.h"

USTRUCT(BlueprintType)
struct FBuildingPartsData : public FTableRowBase
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector MeshLocationOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector SpawnLocationOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Scale;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABuildingPartsBase> Class;
};

class UPrimitiveComponent;
USTRUCT(BlueprintType)
struct FBuildBox
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BuildingPartsName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UPrimitiveComponent*> BoxArray;
};

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class MARINECRAFT_API ABuildingPartsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPartsBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetBuildingPartsName(UPrimitiveComponent* TargetComponent) const;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UBoxComponent* BoxComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TArray<FBuildBox> BuildBoxArray;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UStaticMeshComponent* StaticMeshComponent;
};