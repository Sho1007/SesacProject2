// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MarineCraft.h"
#include "GameFramework/GameModeBase.h"
#include "InGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MARINECRAFT_API AInGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};