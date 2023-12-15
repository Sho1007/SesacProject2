// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusWidget.generated.h"

/**
 * 
 */

class UProgressBar;
class UStatusComponent;
UCLASS()
class MARINECRAFT_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(UStatusComponent* NewStatusComponent);

	UFUNCTION()
	void UpdateThirst(float CurrentThirst, float MaxThirst);
	UFUNCTION()
	void UpdateHunger(float CurrentHunger, float MaxHunger);
	UFUNCTION()
	void UpdateHealth(float CurrentHealth, float MaxHealth);

private:
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	UProgressBar* ProgressBar_Thirst;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	UProgressBar* ProgressBar_Hunger;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	UProgressBar* ProgressBar_Health;
};