// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CLASS_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define LOG(fmt, ...) UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *CLASS_INFO, *FString::Printf(fmt, ##__VA_ARGS__))