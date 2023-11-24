// Fill out your copyright notice in the Description page of Project Settings.


#include "../Tool/ToolBase.h"

void AToolBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentDurability = MaxDurability;
}

void AToolBase::Use()
{
}

void AToolBase::StopUse()
{
}

void AToolBase::Cancel()
{
}

float AToolBase::GetCurrentDurability()
{
	return CurrentDurability;
}

float AToolBase::GetMaxDurability()
{
	return MaxDurability;
}