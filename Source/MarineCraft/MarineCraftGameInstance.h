// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MarineCraftGameInstance.generated.h"

/**
 * 
 */
class UDataTable;
struct FBuildingPartsData;
struct FItemData;
UCLASS()
class MARINECRAFT_API UMarineCraftGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FBuildingPartsData* GetBuildingPartsData(FName BuildingPartsName) const;
	FItemData* GetItemData(FName ItemName) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UDataTable* BuildingPartsDataTable;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	UDataTable* ItemDataTable;
};