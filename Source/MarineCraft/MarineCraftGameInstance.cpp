// Fill out your copyright notice in the Description page of Project Settings.


#include "MarineCraftGameInstance.h"

#include "Building/BuildingPartsBase.h"

FBuildingPartsData* UMarineCraftGameInstance::GetBuildingPartsData(FName BuildingPartsName) const
{
	check(BuildingPartsDataTable);

	return BuildingPartsDataTable->FindRow<FBuildingPartsData>(BuildingPartsName, TEXT(""));
}