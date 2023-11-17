// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ToolBase.generated.h"

UCLASS()
class MARINECRAFT_API AToolBase : public AItemBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getter / Setter
	bool IsChargeable() const;

protected:
	UPROPERTY(VisibleInstanceOnly)
	bool bIsChargeable;
};